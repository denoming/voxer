#include "VoxerImpl.hpp"

#include "Types.hpp"

#include <piper-phonemize/phonemize.hpp>
#include <spdlog/spdlog.h>
#include <utf8cpp/utf8.h>

namespace jar {

void
VoxerImpl::configure(const std::string& filesPath,
                     const std::string& modelPath,
                     bool useCuda,
                     SpeakerId speakerId)
{
    // Set up espeak-ng for calling espeak_TextToPhonemesWithTerminator in "piper-phonemize"
    // See: https://github.com/rhasspy/espeak-ng
    _speakNgLoader = std::make_unique<SpeakNgLoader>(filesPath);

    _model = std::make_unique<Model>(modelPath, useCuda, speakerId);

    _synthesizer = std::make_unique<Synthesizer>(*_model);
}

void
VoxerImpl::cleanup()
{
    _speakNgLoader.reset();
    _model.reset();
    _synthesizer.reset();
}

SynthesisResult
VoxerImpl::textToAudio(std::string text, AudioBuffer& buffer, const Callback& callback)
{
    SynthesisResult result;

    const SynthesisConfig& synthesisConfig = _model->synthesisConfig();
    const PhonemizeConfig& phonemizeConfig = _model->phonemizeConfig();

    std::size_t silenceSamples{};
    if (synthesisConfig.sentenceSilenceSeconds > 0) {
        silenceSamples = static_cast<std::size_t>(synthesisConfig.sentenceSilenceSeconds
                                                  * static_cast<float>(synthesisConfig.sampleRate)
                                                  * static_cast<float>(synthesisConfig.channels));
    }

    // Phonemes for each sentence
    SPDLOG_DEBUG("Phonemizing text: {}", text);
    std::vector<Phonemes> phonemes;

    if (phonemizeConfig.phonemeType == eSpeakPhonemes) {
        // Use espeak-ng for phonemization
        piper::eSpeakPhonemeConfig eSpeakConfig;
        eSpeakConfig.voice = phonemizeConfig.eSpeak.voice;
        phonemize_eSpeak(std::move(text), eSpeakConfig, phonemes);
    } else {
        // Use UTF-8 codepoints as "phonemes"
        piper::CodepointsPhonemeConfig codepointsConfig;
        piper::phonemize_codepoints(std::move(text), codepointsConfig, phonemes);
    }

    // Synthesize each sentence independently.
    PhonemeIds phonemeIds;
    std::map<piper::Phoneme, std::size_t> missingPhonemes;
    for (auto& sentencePhonemes : phonemes) {
        if (spdlog::should_log(spdlog::level::debug)) {
            std::string phonemesStr;
            for (auto phoneme : sentencePhonemes) {
                utf8::append(phoneme, std::back_inserter(phonemesStr));
            }
            spdlog::debug("Converting <{}> phoneme(s) to ids: <{}>",
                          std::size(sentencePhonemes),
                          phonemesStr);
        }

        std::vector<std::shared_ptr<Phonemes>> allPhrasePhonemes;
        std::vector<SynthesisResult> phraseResults;
        std::vector<size_t> phraseSilenceSamples;

        // Use phoneme/id map from config
        piper::PhonemeIdConfig idConfig;
        idConfig.phonemeIdMap = std::make_shared<PhonemeIdsMap>(phonemizeConfig.phonemeIdsMap);

        if (synthesisConfig.phonemeSilenceSeconds) {
            // Split into phrases
            auto& phonemeSilenceSeconds = synthesisConfig.phonemeSilenceSeconds.value();

            auto phrasePhonemes = std::make_shared<Phonemes>();
            allPhrasePhonemes.push_back(phrasePhonemes);

            for (const char32_t phoneme : sentencePhonemes) {
                phrasePhonemes->push_back(phoneme);
                if (auto phonemeIt = phonemeSilenceSeconds.find(phoneme);
                    phonemeIt != std::cend(phonemeSilenceSeconds)) {
                    // Split at phrase boundary
                    const auto [_, len] = *phonemeIt;
                    phraseSilenceSamples.push_back(static_cast<std::size_t>(len)
                                                   * synthesisConfig.sampleRate
                                                   * synthesisConfig.channels);

                    phrasePhonemes = std::make_shared<Phonemes>();
                    allPhrasePhonemes.push_back(phrasePhonemes);
                }
            }
        } else {
            // Use all phonemes
            allPhrasePhonemes.push_back(
                std::make_shared<std::vector<piper::Phoneme>>(sentencePhonemes));
        }

        // Ensure results/samples are the same size
        while (phraseResults.size() < allPhrasePhonemes.size()) {
            phraseResults.emplace_back();
        }

        while (phraseSilenceSamples.size() < allPhrasePhonemes.size()) {
            phraseSilenceSamples.push_back(0);
        }

        // Phonemes -> ids -> Audio
        for (size_t phraseIdx = 0; phraseIdx < allPhrasePhonemes.size(); phraseIdx++) {
            const auto& phrasePhoneme = allPhrasePhonemes[phraseIdx];
            if (phrasePhoneme->empty()) {
                continue;
            }

            // Phonemes -> ids
            piper::phonemes_to_ids(*phrasePhoneme, idConfig, phonemeIds, missingPhonemes);
            if (spdlog::should_log(spdlog::level::debug)) {
                std::stringstream phonemeIdsStr;
                for (auto phonemeId : phonemeIds) {
                    phonemeIdsStr << phonemeId << ", ";
                }
                spdlog::debug("Converted <{}> phoneme(s) to <{}> phoneme id(s): <{}>",
                              allPhrasePhonemes[phraseIdx]->size(),
                              phonemeIds.size(),
                              phonemeIdsStr.str());
            }

            // Ids -> audio
            phraseResults[phraseIdx] = _synthesizer->synthesize(phonemeIds, buffer);

            // Add end of phrase silence
            for (std::size_t i = 0; i < phraseSilenceSamples[phraseIdx]; i++) {
                buffer.push_back(0);
            }

            result.audioSeconds += phraseResults[phraseIdx].audioSeconds;
            result.inferSeconds += phraseResults[phraseIdx].inferSeconds;

            phonemeIds.clear();
        }

        // Add end of sentence silence
        if (silenceSamples > 0) {
            for (std::size_t i = 0; i < silenceSamples; i++) {
                buffer.push_back(0);
            }
        }

        if (callback) {
            callback();
            buffer.clear();
        }

        phonemeIds.clear();
    }

    if (not missingPhonemes.empty()) {
        SPDLOG_WARN("Missing <{}> phoneme(s) from phoneme/id map", std::size(missingPhonemes));
        for (auto [phoneme, size] : missingPhonemes) {
            std::string phonemeStr;
            utf8::append(phoneme, std::back_inserter(phonemeStr));
            SPDLOG_WARN("Missing <{} (\\u{:04X})>: <{}> time(s)",
                        phonemeStr,
                        static_cast<uint32_t>(phoneme),
                        size);
        }
    }

    if (result.audioSeconds > 0) {
        result.realTimeFactor = result.inferSeconds / result.audioSeconds;
    }

    return result;
}

} // namespace jar
