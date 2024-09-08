#pragma once

#include "Types.hpp"
#include "VoxerExport.hpp"

#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace jar {

class VOXER_EXPORT Voxer {
public:
    using Callback = std::function<void()>;

    Voxer();

    ~Voxer();

    void
    configure(const std::string& modelPath,
              bool useCuda = false,
              SpeakerId speakerId = SpeakerId::Default);

    void
    configure(const std::string& filesPath,
              const std::string& modelPath,
              bool useCuda = false,
              SpeakerId speakerId = SpeakerId::Default);

    void
    cleanup();

    SynthesisResult
    textToAudio(std::string text, std::vector<int16_t>& buffer, const Callback& callback = nullptr);

private:
    std::unique_ptr<class VoxerImpl> _impl;
};

} // namespace jar
