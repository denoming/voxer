// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#pragma once

namespace jar {

enum class SpeakerId {
    Default = -1,
};

struct SynthesisResult {
    double inferSeconds{};
    double audioSeconds{};
    double realTimeFactor{};
};

enum class DataFormat { Wav };

} // namespace jar
