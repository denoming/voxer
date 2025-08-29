// Copyright 2025 Denys Asauliak
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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