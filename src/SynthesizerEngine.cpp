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