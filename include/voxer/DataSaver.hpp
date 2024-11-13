// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#pragma once

#include "voxer/VoxerExport.hpp"
#include "voxer/DataHandler.hpp"

#include <memory>
#include <filesystem>

namespace jar {

class VOXER_EXPORT DataSaver final : public DataHandler {
public:
    explicit DataSaver(std::filesystem::path outputFile);

    ~DataSaver() override;

private:
    void
    onBegin(int sampleRate, int sampleWidth, int channels) override;

    void
    onData(const void* ptr, unsigned int size) override;

    void
    onEnd(const SynthesisResult& result) override;

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace jar
