#pragma once

#include "voxer/Types.hpp"
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