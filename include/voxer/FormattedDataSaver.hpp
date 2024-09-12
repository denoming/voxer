// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#pragma once

#include <voxer/VoxerExport.hpp>
#include "voxer/DataHandler.hpp"

#include <filesystem>

namespace jar {

class VOXER_EXPORT FormattedDataSaver final : public DataHandler {
public:
    explicit FormattedDataSaver(DataFormat format, std::filesystem::path outputFile);

    ~FormattedDataSaver() override;

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