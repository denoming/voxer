// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#pragma once

#include "voxer/Types.hpp"
#include "voxer/DataHandler.hpp"

#include "Model.hpp"
#include "Synthesizer.hpp"
#include "SpeakNgLoader.hpp"

#include <string>
#include <memory>

namespace jar {

class VoxerImpl {
public:
    VoxerImpl() = default;

    void
    configure(const std::string& filesPath,
              const std::string& modelPath,
              bool useCuda = false,
              SpeakerId speakerId = SpeakerId::Default);

    void
    cleanup();

    [[nodiscard]] SynthesisResult
    textToAudio(std::string text, DataHandler& handler);

private:
    std::unique_ptr<SpeakNgLoader> _speakNgLoader;
    std::unique_ptr<Model> _model;
    std::unique_ptr<Synthesizer> _synthesizer;
};

} // namespace jar
