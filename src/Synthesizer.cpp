// Copyright 2025 Denys Asauliak
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "Synthesizer.hpp"
#include "Model.hpp"
#include "TextTranslator.hpp"
#include "PhonemesMatcher.hpp"

#include "voxer/Types.hpp"
#include "voxer/DataHandler.hpp"

#include <spdlog/spdlog.h>
#include <utf8cpp/utf8.h>

#include <chrono>
#include <limits>
#include <algorithm>
#include <functional>

namespace krono = std::chrono;

namespace jar {

namespace {

float
getScalingFactor(const float* audio, std::size_t size)
{
    // Maximum value for 16-bit signed WAV sample
    constexpr float kMaxValue = 32767.0f;

    float maxAudioValue = 0.01f;
    for (int64_t n = 0; n < size; n++) {
        if (const float sample = abs(audio[n]); sample > maxAudioValue) {
            maxAudioValue = sample;
        }
    }

    // Scale audio to fill range and convert to int16
    return (kMaxValue / std::max(0.01f, maxAudioValue));
}

template<typename Ret>
Ret
scale(const float input, const float scaleFactor)
{
    return static_cast<Ret>(std::clamp(input * scaleFactor,
                                       static_cast<float>(std::numeric_limits<Ret>::min()),
                                       static_cast<float>(std::numeric_limits<Ret>::max())));
}

} // namespace

Synthesizer::Synthesizer(Model& model)
    : _model{model}
{
}

SynthesisResult
Synthesizer::synthesize(std::string text, DataHandler& handler)
{
    const SynthesisConfig& sConfig = _model.synthesisConfig();
    const PhonemizeConfig& pConfig = _model.phonemizeConfig();

    std::size_t silenceSamples{};
    if (sConfig.sentenceSilenceSeconds > 0) {
        silenceSamples = static_cast<std::size_t>(sConfig.sentenceSilenceSeconds
                                                  * static_cast<float>(sConfig.sampleRate)
                                                  * static_cast<float>(sConfig.channels));
    }

    PhonemeSentences sentences;
    if (pConfig.phonemeType == eSpeakPhonemes) {
        SPDLOG_DEBUG("Phonemizing text (sentences): {}", text);
        eSpeakPhonemeConfig eSpeakConfig;
        eSpeakConfig.voice = pConfig.eSpeak.voice;
        sentences = TextTranslator{text, eSpeakConfig}.release();
    } else {
        SPDLOG_DEBUG("Phonemizing text (codepoints): {}", text);
        CodepointsPhonemeConfig codepointsConfig;
        sentences = TextTranslator{text, codepointsConfig}.release();
    }

    handler.onBegin(sConfig.sampleRate, sConfig.sampleWidth, sConfig.channels);

    // Synthesize each sentence independently.
    SynthesisResult result;
    for (const PhonemeSentence& sentence : sentences) {
        std::vector<size_t> phraseSilenceSamples;

        // Use phoneme/id map from config
        PhonemeIdConfig idConfig;
        idConfig.phonemeIdMap = std::make_shared<PhonemeIdsMap>(pConfig.phonemeIdsMap);

        std::vector<std::shared_ptr<Phonemes>> allPhrasePhonemes;
        if (sConfig.phonemeSilenceSeconds) {
            // Split into phrases
            auto& phonemeSilenceSeconds = sConfig.phonemeSilenceSeconds.value();

            auto phrasePhonemes = std::make_shared<Phonemes>();
            allPhrasePhonemes.push_back(phrasePhonemes);

            for (const char32_t phoneme : sentence) {
                phrasePhonemes->push_back(phoneme);
                if (auto phonemeIt = phonemeSilenceSeconds.find(phoneme);
                    phonemeIt != std::cend(phonemeSilenceSeconds)) {
                    // Split at phrase boundary
                    const auto [_, len] = *phonemeIt;
                    phraseSilenceSamples.push_back(static_cast<std::size_t>(len)
                                                   * sConfig.sampleRate * sConfig.channels);

                    phrasePhonemes = std::make_shared<Phonemes>();
                    allPhrasePhonemes.push_back(phrasePhonemes);
                }
            }
        } else {
            // Use all phonemes
            allPhrasePhonemes.push_back(std::make_shared<Phonemes>(sentence));
        }

        // Ensure results/samples are the same size
        std::vector<SynthesisResult> phraseResults;
        while (phraseResults.size() < allPhrasePhonemes.size()) {
            phraseResults.emplace_back();
        }

        while (phraseSilenceSamples.size() < allPhrasePhonemes.size()) {
            phraseSilenceSamples.push_back(0);
        }

        { // Phonemes => IDs => Audio
            AudioBuffer buffer;
            for (size_t phraseIdx = 0; phraseIdx < allPhrasePhonemes.size(); phraseIdx++) {
                const auto& phrasePhoneme = allPhrasePhonemes[phraseIdx];
                if (phrasePhoneme->empty()) {
                    continue;
                }

                PhonemeIds phonemeIds = PhonemesMatcher{*phrasePhoneme, idConfig}.release();
                phraseResults[phraseIdx] = synthesize(phonemeIds, buffer);
                for (std::size_t i = 0; i < phraseSilenceSamples[phraseIdx]; i++) {
                    buffer.push_back(0);
                }
                phonemeIds.clear();

                result.audioSeconds += phraseResults[phraseIdx].audioSeconds;
                result.inferSeconds += phraseResults[phraseIdx].inferSeconds;
            }

            // Add end of sentence silence
            if (silenceSamples > 0) {
                for (std::size_t i = 0; i < silenceSamples; i++) {
                    buffer.push_back(0);
                }
            }

            handler.onData(buffer.data(), buffer.size());
        }
    }

    if (result.audioSeconds > 0) {
        result.realTimeFactor = result.inferSeconds / result.audioSeconds;
    }

    handler.onEnd(result);

    return result;
}

SynthesisResult
Synthesizer::synthesize(PhonemeIds& phonemeIds, AudioBuffer& audioBuffer) const
{
    SPDLOG_DEBUG("Synthesizing audio for <{}> phoneme id(s)", std::size(phonemeIds));

    SynthesisResult result;

    // Allocate memory
    auto memoryInfo = Ort::MemoryInfo::CreateCpu(OrtAllocatorType::OrtArenaAllocator,
                                                 OrtMemType::OrtMemTypeDefault);

    // Allocate tensors
    const SynthesisConfig& config = _model.synthesisConfig();

    std::vector<Ort::Value> inputTensors;
    const std::vector<int64_t> phonemeIdsShape{1, std::ssize(phonemeIds)};
    inputTensors.push_back(Ort::Value::CreateTensor<int64_t>(memoryInfo,
                                                             std::data(phonemeIds),
                                                             std::size(phonemeIds),
                                                             std::data(phonemeIdsShape),
                                                             std::size(phonemeIdsShape)));

    std::vector<int64_t> phonemeIdLens{std::ssize(phonemeIds)};
    const std::vector<int64_t> phomemeIdLensShape{std::ssize(phonemeIdLens)};
    inputTensors.push_back(Ort::Value::CreateTensor<int64_t>(memoryInfo,
                                                             std::data(phonemeIdLens),
                                                             std::size(phonemeIdLens),
                                                             std::data(phomemeIdLensShape),
                                                             std::size(phomemeIdLensShape)));

    std::vector<float> scales{config.noiseScale, config.lengthScale, config.noiseW};
    std::vector<int64_t> scalesShape{std::ssize(scales)};
    inputTensors.push_back(Ort::Value::CreateTensor<float>(memoryInfo,
                                                           std::data(scales),
                                                           std::size(scales),
                                                           std::data(scalesShape),
                                                           std::size(scalesShape)));

    // Add speaker id.
    std::vector<int64_t> speakerId{static_cast<int64_t>(config.speakerId.value_or(SpeakerId{0}))};
    std::vector<int64_t> speakerIdShape{std::ssize(speakerId)};
    if (config.speakerId) {
        inputTensors.push_back(Ort::Value::CreateTensor<int64_t>(memoryInfo,
                                                                 std::data(speakerId),
                                                                 std::size(speakerId),
                                                                 std::data(speakerIdShape),
                                                                 std::size(speakerIdShape)));
    }

    // Inference
    constexpr std::array inputNames = {"input", "input_lengths", "scales", "sid"};
    constexpr std::array outputNames = {"output"};

    const auto startTime = krono::steady_clock::now();
    auto outputTensors = _model.inference(Ort::RunOptions{nullptr},
                                          std::data(inputNames),
                                          std::data(inputTensors),
                                          std::size(inputTensors),
                                          std::data(outputNames),
                                          std::size(outputNames));
    if ((std::size(outputTensors) != 1) or (not outputTensors.front().IsTensor())) {
        throw std::runtime_error("Invalid output tensors");
    }
    const auto endTime = krono::steady_clock::now();

    const auto* audio = outputTensors.front().GetTensorData<float>();
    const auto audioShape = outputTensors.front().GetTensorTypeAndShapeInfo().GetShape();
    const int64_t audioCount = audioShape[audioShape.size() - 1];

    result.inferSeconds = krono::duration<double>(endTime - startTime).count();
    result.audioSeconds = static_cast<double>(audioCount) / static_cast<double>(config.sampleRate);
    if (result.realTimeFactor = 0.0; result.audioSeconds > 0) {
        result.realTimeFactor = result.inferSeconds / result.audioSeconds;
    }

    SPDLOG_DEBUG("Synthesized <{}> second(s) of audio in <{}> second(s)",
                 result.audioSeconds,
                 result.inferSeconds);

    audioBuffer.reserve(audioCount);
    std::transform(audio,
                   audio + audioCount,
                   std::back_inserter(audioBuffer),
                   [scaleFactor = getScalingFactor(audio, audioCount)](const float input) {
                       return scale<int16_t>(input, scaleFactor);
                   } /* Back insert with scaling */);

    // Clean up tensors
    for (auto& outputTensor : outputTensors) {
        Ort::detail::OrtRelease(outputTensor.release());
    }
    for (auto& inputTensor : inputTensors) {
        Ort::detail::OrtRelease(inputTensor.release());
    }

    // Clean up memory
    memoryInfo.release();

    return result;
}

} // namespace jar