#pragma once

#include "voxer/Types.hpp"
#include "voxer/FileSaver.hpp"

#include "Model.hpp"
#include "Synthesizer.hpp"
#include "SpeakNgLoader.hpp"

#include <string>
#include <functional>
#include <memory>

namespace jar {

class VoxerImpl {
public:
    using PrerollCallback = std::function<void(int sampleRate, int sampleSize, int channels)>;
    using BufferCallback = std::function<void(AudioBuffer& buffer)>;

    VoxerImpl() = default;

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

    [[nodiscard]] SynthesisResult
    textToAudio(std::string text, AudioBuffer& buffer);

private:
    [[nodiscard]] bool
    notifyPreroll(int sampleRate, int sampleSize, int channels) const;

    [[nodiscard]] bool
    notifyBuffer(AudioBuffer& buffer) const;

private:
    PrerollCallback _prerollCallback;
    BufferCallback _bufferCallback;
    std::unique_ptr<SpeakNgLoader> _speakNgLoader;
    std::unique_ptr<Model> _model;
    std::unique_ptr<Synthesizer> _synthesizer;
};

} // namespace jar
