// Copyright 2025 Denys Asauliak
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
     * Uses \p VOXER_MODEL_FILE env variable to retrieve path to model file.\n
     * Uses \p VOXER_ESPEAK_DIR env variable to retrieve path to files dir.
     * @param useCuda the flag to use CUDA or not
     * @param speakerId the index of speaker to use (see *.onnx.json model config file)
     */
    [[nodiscard]] bool
    configure(bool useCuda = false, SpeakerId speakerId = SpeakerId::Default);

    /**
     * Configures library.\n
     * Uses \p VOXER_ESPEAK_DIR env variable to retrieve path to files dir.
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

    /**
     * Returns model synthesis config information.
     * @return the string with information
     */
    [[nodiscard]] std::string
    info() const;

public:
    /**
     * Return the application version
     * @return the string with version
     */
    [[nodiscard]] static std::string
    version();

private:
    std::unique_ptr<class VoxerImpl> _impl;
};

} // namespace jar
