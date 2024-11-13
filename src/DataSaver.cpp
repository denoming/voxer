// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#include "voxer/DataSaver.hpp"

#include <fstream>

namespace fs = std::filesystem;

namespace jar {

class DataSaver::Impl {
public:
    explicit Impl(fs::path outputFile)
        : _outputFile{std::move(outputFile)}
    {
    }

    void
    open()
    {
        _ofs = std::ofstream{_outputFile};
    }

    void
    write(const void* ptr, const unsigned int size)
    {
        if (_ofs) {
            _ofs.write(static_cast<const char*>(ptr), size);
        }
    }

    void
    close()
    {
        if (_ofs) {
            _ofs.flush();
            _ofs.close();
        }
    }

private:
    std::filesystem::path _outputFile;
    std::ofstream _ofs;
};

DataSaver::DataSaver(fs::path outputFile)
    : _impl{std::make_unique<Impl>(std::move(outputFile))}
{
}

DataSaver::~DataSaver() = default;

void
DataSaver::onBegin(int /*sampleRate*/, int /*sampleWidth*/, int /*channels*/)
{
    _impl->open();
}

void
DataSaver::onData(const void* ptr, unsigned int size)
{
    _impl->write(ptr, size);
}

void
DataSaver::onEnd(const SynthesisResult& /*result*/)
{
    _impl->close();
}

} // namespace jar