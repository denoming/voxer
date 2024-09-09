// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#include "WavFileSaver.hpp"

#include <fmt/format.h>
#include <fmt/std.h>

namespace jar {

bool
WavFileSaver::isOpened() const
{
    return _handle;
}

void
WavFileSaver::open(const std::filesystem::path& outputFile,
                   const int sampleRate,
                   const int sampleSize,
                   const int channels)
{

    int format{SF_FORMAT_WAV};

    switch (sampleSize) {
    case 2:
        format |= SF_FORMAT_PCM_16;
        break;
    case 3:
        format |= SF_FORMAT_PCM_24;
        break;
    case 4:
        format |= SF_FORMAT_PCM_16;
        break;
    default:
        throw std::runtime_error("Unsupported sample size");
    }

    _handle = SndfileHandle{outputFile, SFM_WRITE, format, channels, sampleRate};
    if (not _handle) {
        throw std::runtime_error{fmt::format(
            "Unable to open <{}> file for writing: {}", outputFile, _handle.strError())};
    }
}

void
WavFileSaver::write(const AudioBuffer& buffer)
{
    if (_handle) {
        _handle.write(buffer.data(), static_cast<sf_count_t>(buffer.size()));
    }
}

void
WavFileSaver::close()
{
    if (_handle) {
        _handle.writeSync();
    }
    _handle = {};
}

} // namespace jar