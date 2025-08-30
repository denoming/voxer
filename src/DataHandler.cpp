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

#include "voxer/DataHandler.hpp"

#include <spdlog/spdlog.h>

namespace jar {

void
DataHandler::onBegin(int sampleRate, int sampleSize, int channels)
{
    SPDLOG_DEBUG(
        "onBegin(sampleRate<{}>, sampleSize<{}>, channels<{}>)", sampleRate, sampleSize, channels);
}

void
DataHandler::onData(const void* ptr, unsigned int size)
{
    SPDLOG_DEBUG("onData(buffer<ptr: {}, size: {}>)", fmt::ptr(ptr), size);
}

void
DataHandler::onEnd(const SynthesisResult& result)
{
    SPDLOG_DEBUG("onEnd(inferSeconds<{}>, audioSeconds<{}>, realTimeFactor<{}>)",
                 result.inferSeconds,
                 result.audioSeconds,
                 result.realTimeFactor);
}

} // namespace jar