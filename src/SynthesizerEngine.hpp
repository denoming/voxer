// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#pragma once

#include <string>
#include <string_view>

namespace jar {

class SynthesizerEngine {
public:
    [[nodiscard]] static int
    setup(const std::string& filesPath);

    [[maybe_unused]] static bool
    cleanup();

    [[nodiscard]] static bool
    setVoice(std::string_view voice);

    [[nodiscard]] static const char*
    textToPhonemes(const char** textPtr);
};

} // namespace jar