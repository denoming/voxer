// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#include "TextTranslator.hpp"
#include "SynthesizerEngine.hpp"

#include <uni_algo/all.h>

#include <map>
#include <ranges>
#include <memory>
#include <stdexcept>

static std::map<std::string, jar::PhonemesMap> kDefaultPhonemesMap = {
    {"pt-br", {{U'c', {U'k'}}}},
};

namespace jar {

TextTranslator::TextTranslator(const std::string& text, const eSpeakPhonemeConfig& config)
{
    translate(text, config);
}

TextTranslator::TextTranslator(const std::string& text, const CodepointsPhonemeConfig& config)
{
    translate(text, config);
}

void
TextTranslator::translate(const std::string& text, const eSpeakPhonemeConfig& config)
{
    if (not SynthesizerEngine::setVoice(config.voice)) {
        throw std::runtime_error("Error to set voice");
    }

    std::shared_ptr<PhonemesMap> phonemeMap;
    if (config.phonemeMap) {
        phonemeMap = config.phonemeMap;
    } else if (kDefaultPhonemesMap.contains(config.voice)) {
        phonemeMap = std::make_shared<PhonemesMap>(kDefaultPhonemesMap[config.voice]);
    }

    for (const char* inputTextPointer = text.data(); inputTextPointer != nullptr;) {
        const char* ptr = SynthesizerEngine::textToPhonemes(&inputTextPointer);
        if (not ptr) {
            throw std::runtime_error{"Error to convert text to phonemes"};
        }

        // Decompose, e.g. "ç" -> "c" + "̧"
        auto phonemesNorm = una::norm::to_nfd_utf8(ptr);
        auto phonemesView = una::ranges::utf8_view{phonemesNorm};

        Phonemes mappedSentPhonemes;
        if (phonemeMap) {
            for (Phoneme phoneme : phonemesView) {
                if (not phonemeMap->contains(phoneme)) {
                    mappedSentPhonemes.push_back(phoneme);
                } else {
                    const Phonemes& mappedPhonemes = phonemeMap->at(phoneme);
                    std::ranges::copy(mappedPhonemes,
                                      std::back_insert_iterator{mappedSentPhonemes});
                }
            }
        } else {
            std::ranges::copy(phonemesView, std::back_insert_iterator{mappedSentPhonemes});
        }

        if (Phonemes& sentencePhonemes = _sentences.emplace_back(); config.keepLanguageFlags) {
            std::ranges::copy(mappedSentPhonemes, std::back_insert_iterator{sentencePhonemes});
        } else {
            bool inLanguageFlag = false;
            for (auto it = std::cbegin(mappedSentPhonemes); it != std::cend(mappedSentPhonemes);) {
                if (inLanguageFlag) {
                    if (*it == U')') {
                        inLanguageFlag = false;
                    }
                } else if (*it == U'(') {
                    inLanguageFlag = true;
                } else {
                    sentencePhonemes.push_back(*it);
                }
                ++it;
            }
        }
    }
}

void
TextTranslator::translate(const std::string& text, const CodepointsPhonemeConfig& config)
{
    std::string casedText;
    switch (config.casing) {
    case TextCasing::Lower:
        casedText = una::cases::to_lowercase_utf8(text);
        break;
    case TextCasing::Upper:
        casedText = una::cases::to_uppercase_utf8(text);
        break;
    case TextCasing::Fold:
        casedText = una::cases::to_casefold_utf8(text);
        break;
    default:
        casedText = text;
    }

    // Decompose, e.g. "ç" -> "c" + "̧"
    auto phonemesNorm = una::norm::to_nfd_utf8(casedText);
    auto phonemesView = una::ranges::utf8_view{phonemesNorm};

    Phonemes& sentPhonemes = _sentences.emplace_back();
    if (config.phonemeMap) {
        for (auto phoneme : phonemesView) {
            if (not config.phonemeMap->contains(phoneme)) {
                sentPhonemes.push_back(phoneme);
            } else {
                const Phonemes& mappedPhonemes = config.phonemeMap->at(phoneme);
                std::ranges::copy(mappedPhonemes, std::back_insert_iterator{sentPhonemes});
            }
        }
    } else {
        std::ranges::copy(phonemesView, std::back_insert_iterator{sentPhonemes});
    }
}

PhonemeSentences::const_iterator
TextTranslator::begin() const
{
    return std::cbegin(_sentences);
}

PhonemeSentences::const_iterator
TextTranslator::end() const
{
    return std::cend(_sentences);
}

bool
TextTranslator::empty() const
{
    return _sentences.empty();
}

PhonemeSentences
TextTranslator::release()
{
    return std::move(_sentences);
}

} // namespace jar