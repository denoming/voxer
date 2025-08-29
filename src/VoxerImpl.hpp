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
#include "voxer/DataHandler.hpp"

#include "Model.hpp"
#include "Synthesizer.hpp"

#include <string>
#include <memory>
#include <filesystem>

namespace jar {

class VoxerImpl {
public:
    VoxerImpl() = default;

    [[nodiscard]] bool
    configure(bool useCuda = false, SpeakerId speakerId = SpeakerId::Default);

    [[nodiscard]] bool
    configure(const std::filesystem::path& modelPath,
              bool useCuda = false,
              SpeakerId speakerId = SpeakerId::Default);

    [[nodiscard]] bool
    configure(const std::filesystem::path& modelPath,
              const std::filesystem::path& filesPath,
              bool useCuda = false,
              SpeakerId speakerId = SpeakerId::Default);

    void
    cleanup();

    [[nodiscard]] SynthesisResult
    textToAudio(std::string text, DataHandler& handler);

    [[nodiscard]] std::string
    info() const;

private:
    std::unique_ptr<Model> _model;
    std::unique_ptr<Synthesizer> _synthesizer;
};

} // namespace jar
