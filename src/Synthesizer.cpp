// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#include "Synthesizer.hpp"

#include <spdlog/spdlog.h>

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
    : _model(model)
{
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