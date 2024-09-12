// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#pragma once

#include "Model.hpp"

namespace jar {

class Synthesizer {
public:
    explicit Synthesizer(Model& model);

    [[maybe_unused]] SynthesisResult
    synthesize(PhonemeIds& phonemeIds, AudioBuffer& audioBuffer) const;

private:
    Model& _model;
};

} // namespace jar