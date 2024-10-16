// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#include "voxer/DataHandler.hpp"

#include <spdlog/spdlog.h>

namespace jar {

void
DataHandler::onBegin(int sampleRate, int sampleSize, int channels)
{
    SPDLOG_DEBUG("onBegin(sampleRate<{}>, sampleSize<{}>, channels<{}>)",
                 sampleRate,
                 sampleSize,
                 channels);
}

void
DataHandler::onData(const void* ptr, unsigned int count)
{
    SPDLOG_DEBUG("onData(buffer<ptr: {}, count: {}>)", fmt::ptr(ptr), count);
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