// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#pragma once

#include <vector>

#include <cstdint>

namespace jar {

enum class SpeakerId {
    Default = -1,
};

struct SynthesisResult {
    double inferSeconds{};
    double audioSeconds{};
    double realTimeFactor{};
};

using AudioBuffer = std::vector<int16_t>;

} // namespace jar
