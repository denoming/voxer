// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#pragma once

#include "voxer/Types.hpp"
#include "voxer/VoxerExport.hpp"
#include "voxer/DataHandler.hpp"

#include <memory>
#include <string>

namespace jar {

class VOXER_EXPORT Voxer {
public:
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
    textToAudio(std::string text, DataHandler& handler);

private:
    std::unique_ptr<class VoxerImpl> _impl;
};

} // namespace jar
