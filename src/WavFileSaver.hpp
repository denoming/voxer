// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#pragma once

#include "voxer/FileSaver.hpp"

#include <sndfile.hh>

namespace jar {

class WavFileSaver final : public FileSaver {
public:
    WavFileSaver() = default;

    [[nodiscard]] bool
    isOpened() const override;

    void
    open(const std::filesystem::path& outputFile,
         int sampleRate,
         int sampleSize,
         int channels) override;

    void
    write(const AudioBuffer& buffer) override;

    void
    close() override;

private:
    SndfileHandle _handle;
};

} // namespace jar