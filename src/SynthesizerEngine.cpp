// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#include "SynthesizerEngine.hpp"

#include <espeak-ng/speak_lib.h>

#include <spdlog/spdlog.h>

namespace jar {

int
SynthesizerEngine::setup(const std::string& filesPath)
{
    return espeak_Initialize(AUDIO_OUTPUT_SYNCHRONOUS, 0, filesPath.data(), 0);
}

bool
SynthesizerEngine::cleanup()
{
    return (espeak_Terminate() == EE_OK);
}

bool
SynthesizerEngine::setVoice(std::string_view voice)
{
    return (espeak_SetVoiceByName(voice.data()) == EE_OK);
}

const char*
SynthesizerEngine::textToPhonemes(const char** textPtr)
{
    return espeak_TextToPhonemes(reinterpret_cast<const void**>(textPtr), espeakCHARS_AUTO, 0x02);
}

} // namespace jar