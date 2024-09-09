// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#pragma once

#include <piper-phonemize/phoneme_ids.hpp>

#include <optional>
#include <vector>
#include <map>
#include <string>

namespace jar {

enum PhonemeType {
    eSpeakPhonemes,
    TextPhonemes,
};

using Phonemes = std::vector<piper::Phoneme>;
using PhonemesMap = std::map<piper::Phoneme, Phonemes>;

using PhonemeIds = std::vector<piper::PhonemeId>;
using PhonemeIdsMap = std::map<piper::Phoneme, PhonemeIds>;

struct eSpeakConfig {
    std::string voice{"en-us"};
};

struct PhonemizeConfig {
    PhonemeType phonemeType = eSpeakPhonemes;
    std::optional<PhonemesMap> phonemesMap;
    PhonemeIdsMap phonemeIdsMap;
    piper::PhonemeId idPad{0}; // padding (optionally interspersed)
    piper::PhonemeId idBos{1}; // beginning of sentence
    piper::PhonemeId idEos{2}; // end of sentence
    bool interspersePad{true};
    eSpeakConfig eSpeak;
};

struct SynthesisConfig {
    // VITS inference settings
    float noiseScale{0.667f};
    float lengthScale{1.0f};
    float noiseW{0.8f};

    // Audio settings
    int sampleRate{22050};
    int sampleWidth{2}; // 16-bit
    int channels{1};    // mono

    // Speaker id from 0 to numSpeakers - 1
    std::optional<SpeakerId> speakerId;

    // Extra silence
    float sentenceSilenceSeconds{0.2f};
    std::optional<std::map<piper::Phoneme, float>> phonemeSilenceSeconds;
};

} // namespace jar
