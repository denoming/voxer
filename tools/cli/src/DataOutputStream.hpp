// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#pragma once

#include "voxer/DataHandler.hpp"

#include <ostream>

namespace jar {

class DataOutputStream final : public DataHandler {
public:
    explicit DataOutputStream(std::ostream& os);

    void
    onBegin(int sampleRate, int sampleWidth, int channels) override;

    void
    onData(const void* ptr, unsigned int size) override;

    void
    onEnd(const SynthesisResult& result) override;

private:
    std::ostream& _os;
};

} // namespace jar
