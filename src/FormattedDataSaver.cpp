// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#include "voxer/FormattedDataSaver.hpp"

#include <sndfile.hh>
#include <spdlog/spdlog.h>

namespace fs = std::filesystem;

namespace jar {

namespace {

int
toSndFormat(const DataFormat /*format*/)
{
    return SF_FORMAT_WAV;
}

} // namespace

class FormattedDataSaver::Impl {
public:
    explicit
    Impl(const DataFormat format, fs::path outputFile)
        : _format{format}
          , _outputFile{std::move(outputFile)}
    {
    }

    void
    open(const int sampleRate, int sampleWidth, const int channels)
    {
        const int kFormat{toSndFormat(_format) | SF_FORMAT_PCM_16};

        if (sampleWidth != 2 /* 2 bytes per sample */) {
            SPDLOG_ERROR("Samples with <{}> width is not supported", sampleWidth);
        }

        _handle = SndfileHandle{_outputFile, SFM_WRITE, kFormat, channels, sampleRate};
        if (not _handle) {
            SPDLOG_ERROR("Unable to create handle: outputFile<{}>, error<{}>",
                         _outputFile.native(),
                         _handle.strError());
            throw std::runtime_error{"Unable to create handle"};
        }
    }

    void
    write(const void* ptr, const unsigned int size)
    {
        _handle.write(static_cast<const short*>(ptr), size);
    }

    void
    finalize(const SynthesisResult& /*result*/)
    {
        if (_handle) {
            _handle = SndfileHandle();
        }
    }

private:
    DataFormat _format{};
    fs::path _outputFile{};
    SndfileHandle _handle;
};

FormattedDataSaver::FormattedDataSaver(DataFormat format, fs::path outputFile)
    : _impl{std::make_unique<Impl>(format, std::move(outputFile))}
{
}

FormattedDataSaver::~FormattedDataSaver() = default;

void
FormattedDataSaver::onBegin(const int sampleRate, const int sampleWidth, const int channels)
{
    _impl->open(sampleRate, sampleWidth, channels);
}

void
FormattedDataSaver::onData(const void* ptr, const unsigned int size)
{
    _impl->write(ptr, size);
}

void
FormattedDataSaver::onEnd(const SynthesisResult& result)
{
    _impl->finalize(result);
}

} // namespace jar