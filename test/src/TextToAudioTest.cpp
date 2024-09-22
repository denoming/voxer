// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <voxer/Voxer.hpp>

static auto kModelPath{"voxer/voices/en_GB-alba-medium.onnx"};
static auto kFilesPath{"espeak-ng-data"};

using namespace jar;
using namespace testing;

class MockDataHandler : public DataHandler {
public:
    MOCK_METHOD(void, onBegin, (int sampleRate, int sampleWidth, int channels), (override));
    MOCK_METHOD(void, onData, (const void* ptr, unsigned int count), (override));
    MOCK_METHOD(void, onEnd, (const SynthesisResult& result), (override));
};

TEST(TextToAudioTest, SynthesizeText)
{
    Voxer voxer;
    EXPECT_NO_THROW(voxer.configure(kFilesPath, kModelPath, false, SpeakerId::Default));

    MockDataHandler handler;
    EXPECT_CALL(handler, onBegin(Gt(0), Gt(0), Gt(0))).Times(1);
    EXPECT_CALL(handler, onData(NotNull(), Gt(0))).Times(AtLeast(1));
    EXPECT_CALL(handler, onEnd).Times(1);
    EXPECT_NO_THROW(voxer.textToAudio("Hello world", handler));

    EXPECT_NO_THROW(voxer.cleanup());
}