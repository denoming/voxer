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
#include <filesystem>

namespace jar {

class VOXER_EXPORT Voxer {
public:
    Voxer();

    ~Voxer();

    /**
     * Configures library.\n
     * Uses \p VOXER_MODEL_DIR env variable to retrieve path to model file.\n
     * Uses \p VOXER_FILES_DIR env variable to retrieve path to files dir.
     * @param useCuda the flag to use CUDA or not
     * @param speakerId the index of speaker to use (see *.onnx.json model config file)
     */
    [[nodiscard]] bool
    configure(bool useCuda = false, SpeakerId speakerId = SpeakerId::Default);

    /**
     * Configures library.\n
     * Uses \p VOXER_FILES_DIR env variable to retrieve path to files dir.
     * @param modelPath the path to model file (*.onnx file)
     * @param useCuda the flag to use CUDA or not
     * @param speakerId the index of speaker to use (see *.onnx.json model config file)
     */
    [[nodiscard]] bool
    configure(const std::filesystem::path& modelPath,
              bool useCuda = false,
              SpeakerId speakerId = SpeakerId::Default);

    /**
     * Configures library.
     * @param filesPath the path to espeak-ng files
     * @param modelPath the path to model file (*.onnx file)
     * @param useCuda the flag to use CUDA or not
     * @param speakerId the index of speaker to use (see *.onnx.json model config file)
     */
    [[nodiscard]] bool
    configure(const std::filesystem::path& modelPath,
              const std::filesystem::path& filesPath,
              bool useCuda = false,
              SpeakerId speakerId = SpeakerId::Default);

    /**
     * Clean-up allocated resources.
     */
    void
    cleanup();

    /**
     * Synthesizes text into speech.
     * @param text the text to synthesize
     * @param handler the handler to obtain the result.
     * @return the result of synthesis.
     */
    SynthesisResult
    textToAudio(std::string text, DataHandler& handler);

private:
    std::unique_ptr<class VoxerImpl> _impl;
};

} // namespace jar
