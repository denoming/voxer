// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#pragma once

#include "voxer/Types.hpp"
#include "voxer/VoxerExport.hpp"
#include "voxer/DataHandler.hpp"

#include <memory>

namespace jar {

class VOXER_EXPORT FormattedDataHandler : public DataHandler {
public:
    explicit FormattedDataHandler(DataFormat format);

    ~FormattedDataHandler() override;

    virtual void
    onFormattedBegin(DataFormat format, int sampleRate, int sampleWidth, int channels)
        = 0;

    virtual void
    onFormattedData(const void* ptr, size_t size)
        = 0;

    virtual void
    onFormattedEnd(const SynthesisResult& result)
        = 0;

private:
    void
    onBegin(int sampleRate, int sampleWidth, int channels) override;

    void
    onData(const void* ptr, unsigned int count) override;

    void
    onEnd(const SynthesisResult& result) override;

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace jar