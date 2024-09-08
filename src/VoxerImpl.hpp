#pragma once

#include "voxer/Types.hpp"

#include "Types.hpp"
#include "Model.hpp"
#include "Synthesizer.hpp"
#include "SpeakNgLoader.hpp"

#include <string>
#include <functional>
#include <memory>

namespace jar {

class VoxerImpl {
public:
    using Callback = std::function<void()>;

    VoxerImpl() = default;

    void
    configure(const std::string& filesPath,
              const std::string& modelPath,
              bool useCuda = false,
              SpeakerId speakerId = SpeakerId::Default);

    void
    cleanup();

    [[nodiscard]] SynthesisResult
    textToAudio(std::string text, AudioBuffer& buffer, const Callback& callback);

private:
    std::unique_ptr<SpeakNgLoader> _speakNgLoader;
    std::unique_ptr<Model> _model;
    std::unique_ptr<Synthesizer> _synthesizer;
};

} // namespace jar
