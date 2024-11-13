// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#pragma once

#include "voxer/Types.hpp"
#include "voxer/VoxerExport.hpp"

namespace jar {

class VOXER_EXPORT DataHandler {
public:
    virtual ~DataHandler() = default;

    virtual void
    onBegin(int sampleRate, int sampleWidth, int channels);

    virtual void
    onData(const void* ptr, unsigned int size);

    virtual void
    onEnd(const SynthesisResult& result);
};

} // namespace jar