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

} // namespace jar
