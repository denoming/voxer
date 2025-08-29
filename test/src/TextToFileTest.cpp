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

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <voxer/Voxer.hpp>
#include <voxer/FormattedDataSaver.hpp>

#include <filesystem>

namespace fs = std::filesystem;

static const auto* kAudioFile{"output.wav"};

using namespace jar;
using namespace testing;

/**
 * Following env variables must be set:
 *  - VOXER_MODEL_FILE
 *  - VOXER_ESPEAK_DIR
 **/
TEST(TextToFileTest, SynthesizeTextWithSaving)
{
    FormattedDataSaver handler{DataFormat::Wav, kAudioFile};

    EXPECT_NO_THROW({
        Voxer voxer;
        ASSERT_TRUE(voxer.configure());
        voxer.textToAudio("Hello world. This world is very beautiful.", handler);
        voxer.cleanup();
    });

    EXPECT_THAT(fs::file_size(kAudioFile), Gt(0));
    fs::remove(kAudioFile);
}
