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
