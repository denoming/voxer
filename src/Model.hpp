// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#pragma once

#include "voxer/Types.hpp"
#include "Types.hpp"

#include <onnxruntime_cxx_api.h>

#include <optional>
#include <string>
#include <map>

namespace jar {

struct ModelConfig {
    int numSpeakers{};

    // speaker name -> id
    std::optional<std::map<std::string, SpeakerId>> speakerIdMap;
};

struct ModelSession {
    Ort::Session onnx;
    Ort::AllocatorWithDefaultOptions allocator;
    Ort::SessionOptions options;
    Ort::Env env;

    ModelSession()
        : onnx{nullptr}
    {
    }
};

class Model {
public:
    explicit Model(const std::string& modelPath,
                   bool useCuda = false,
                   SpeakerId speakerId = SpeakerId::Default);

    Model(const std::string& modelPath,
          const std::string& configPath,
          bool useCuda = false,
          SpeakerId speakerId = SpeakerId::Default);

    [[nodiscard]] const PhonemizeConfig&
    phonemizeConfig() const;

    [[nodiscard]] const SynthesisConfig&
    synthesisConfig() const;

    [[nodiscard]] bool
    isMultiSpeaker() const;

    [[nodiscard]] SpeakerId
    speakerId() const;

    [[nodiscard]] std::vector<Ort::Value>
    inference(const Ort::RunOptions& options,
              const char* const* inNames,
              const Ort::Value* inValues,
              size_t inCount,
              const char* const* outNames,
              size_t outCount);

private:
    void
    loadModel(const std::string& modelPath, bool useCuda);

    void
    loadConfig(const std::string& configPath, SpeakerId speakerId);

private:
    PhonemizeConfig _phonemizeConfig;
    SynthesisConfig _synthesisConfig;
    ModelConfig _modelConfig;
    ModelSession _session;
};

} // namespace jar