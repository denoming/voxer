#pragma once

#include "voxer/Types.hpp"
#include "voxer/VoxerExport.hpp"

#include <memory>
#include <string>
#include <functional>

namespace jar {

class VOXER_EXPORT Voxer {
public:
    using PrerollCallback = std::function<void(int sampleRate, int sampleSize, int channels)>;
    using BufferCallback = std::function<void(AudioBuffer& buffer)>;

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

    void
    onPreroll(PrerollCallback callback);

    void
    onBuffer(BufferCallback callback);

    SynthesisResult
    textToAudio(std::string text, AudioBuffer& buffer);

private:
    std::unique_ptr<class VoxerImpl> _impl;
};

} // namespace jar
