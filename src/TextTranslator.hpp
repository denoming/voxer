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