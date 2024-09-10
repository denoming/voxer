// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#include "SpeakNgLoader.hpp"

#include <espeak-ng/speak_lib.h>

#include <spdlog/spdlog.h>

namespace jar {

SpeakNgLoader::SpeakNgLoader(const std::string& filesPath)
{
    SPDLOG_DEBUG("Initializing eSpeak using <{}> data path", filesPath);
    if (espeak_Initialize(AUDIO_OUTPUT_SYNCHRONOUS, 0, filesPath.data(), 0) < 0) {
        throw std::runtime_error("Unable to initialize eSpeak-ng");
    }
    SPDLOG_DEBUG("Initialized eSpeak");
}

SpeakNgLoader::~SpeakNgLoader()
{
    SPDLOG_DEBUG("Terminating eSpeak");
    espeak_Terminate();
    SPDLOG_DEBUG("Terminated eSpeak");
}

} // namespace jar