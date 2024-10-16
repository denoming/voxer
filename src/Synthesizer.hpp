// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#pragma once

#include "Types.hpp"

#include <string>

namespace jar {

class Model;
class DataHandler;

class Synthesizer {
public:
    explicit Synthesizer(Model& model);

    [[maybe_unused]] SynthesisResult
    synthesize(std::string text, DataHandler& handler);

private:
    [[maybe_unused]] SynthesisResult
    synthesize(PhonemeIds& phonemeIds, AudioBuffer& audioBuffer) const;

private:
    Model& _model;
};

} // namespace jar