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

#include <voxer/Options.hpp>
#include <voxer/Voxer.hpp>

#include <filesystem>

using namespace jar;
using namespace testing;

namespace fs = std::filesystem;

class MockDataHandler : public DataHandler {
public:
    MOCK_METHOD(void, onBegin, (int sampleRate, int sampleWidth, int channels), (override));
    MOCK_METHOD(void, onData, (const void* ptr, unsigned int count), (override));
    MOCK_METHOD(void, onEnd, (const SynthesisResult& result), (override));
};

/**
 * Following env variables must be set:
 *  - VOXER_ESPEAK_DIR
 *    (e.g. "/usr/lib/aarch64-linux-gnu/espeak-ng-data" for arm64 platform)
 *    (e.g. "/usr/lib/x86_64-linux-gnu/espeak-ng-data" for amd64 platform)
 **/
TEST(TextToAudioTest, SynthesizeText)
{
    MockDataHandler handler;
    EXPECT_CALL(handler, onBegin(Gt(0), Gt(0), Gt(0))).Times(1);
    EXPECT_CALL(handler, onData(NotNull(), Gt(0))).Times(AtLeast(1));
    EXPECT_CALL(handler, onEnd).Times(1);

    const fs::path voicesDir{VOXER_VOICES_LIST_DIR};
    ASSERT_TRUE(fs::exists(voicesDir)) << "Dir with voices models does not exist";
    const fs::path modelPath{voicesDir / "en_US-amy-medium.onnx"};
    ASSERT_TRUE(fs::exists(modelPath)) << "Model path does not exist";

    EXPECT_NO_THROW({
        Voxer voxer;
        ASSERT_TRUE(voxer.configure(modelPath));
        voxer.textToAudio("Hello world", handler);
        voxer.cleanup();
    });
}