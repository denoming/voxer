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