// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#pragma once

#include "Types.hpp"

#include <string>

namespace jar {

class TextTranslator {
public:
    TextTranslator() = default;

    TextTranslator(const std::string& text, const eSpeakPhonemeConfig& config);

    TextTranslator(const std::string& text, const CodepointsPhonemeConfig& config);

    void
    translate(const std::string& text, const eSpeakPhonemeConfig& config);

    void
    translate(const std::string& text, const CodepointsPhonemeConfig& config);

    [[nodiscard]] PhonemeSentences::const_iterator
    begin() const;

    [[nodiscard]] PhonemeSentences::const_iterator
    end() const;

    [[nodiscard]] bool
    empty() const;

    PhonemeSentences
    release();

private:
    PhonemeSentences _sentences;
};

} // namespace jar