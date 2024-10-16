// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#pragma once

#include "voxer/Types.hpp"

#include <optional>
#include <vector>
#include <map>
#include <string>
#include <memory>

namespace jar {

enum PhonemeType {
    eSpeakPhonemes,
    TextPhonemes,
};

using Phoneme = char32_t;
using Phonemes = std::vector<Phoneme>;
using PhonemesMap = std::map<Phoneme, Phonemes>;

using PhonemeId = int64_t;
using PhonemeIds = std::vector<PhonemeId>;
using PhonemeIdsMap = std::map<Phoneme, PhonemeIds>;

using PhonemeSentence = std::vector<Phoneme>;
using PhonemeSentences = std::vector<PhonemeSentence>;

using AudioBuffer = std::vector<int16_t>;

enum class TextCasing { Ignore, Lower, Upper, Fold };

struct PhonemeIdConfig {
    Phoneme pad = U'_';
    Phoneme bos = U'^';
    Phoneme eos = U'$';

    // Every other phoneme id is pad
    bool interspersePad = true;

    // Add beginning of sentence (bos) symbol at start
    bool addBos = true;

    // Add end of sentence (eos) symbol at end
    bool addEos = true;

    // Map from phonemes to phoneme id(s).
    // Not set means to use DEFAULT_PHONEME_ID_MAP.
    std::shared_ptr<PhonemeIdsMap> phonemeIdMap;
};

struct CodepointsPhonemeConfig {
    TextCasing casing{TextCasing::Fold};
    std::shared_ptr<PhonemesMap> phonemeMap;
};

struct eSpeakPhonemeConfig {
    std::string voice = "en-us";

    Phoneme period = U'.';      // CLAUSE_PERIOD
    Phoneme comma = U',';       // CLAUSE_COMMA
    Phoneme question = U'?';    // CLAUSE_QUESTION
    Phoneme exclamation = U'!'; // CLAUSE_EXCLAMATION
    Phoneme colon = U':';       // CLAUSE_COLON
    Phoneme semicolon = U';';   // CLAUSE_SEMICOLON
    Phoneme space = U' ';

    // Remove language switch flags like "(en)"
    bool keepLanguageFlags = false;

    std::shared_ptr<PhonemesMap> phonemeMap;
};

struct eSpeakConfig {
    std::string voice{"en-us"};
};

struct PhonemizeConfig {
    PhonemeType phonemeType = eSpeakPhonemes;
    std::optional<PhonemesMap> phonemesMap;
    PhonemeIdsMap phonemeIdsMap;
    PhonemeId idPad{0}; // padding (optionally interspersed)
    PhonemeId idBos{1}; // beginning of sentence
    PhonemeId idEos{2}; // end of sentence
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
    std::optional<std::map<Phoneme, float>> phonemeSilenceSeconds;
};

} // namespace jar
