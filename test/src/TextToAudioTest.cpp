// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <voxer/Voxer.hpp>

using namespace jar;
using namespace testing;

class MockDataHandler : public DataHandler {
public:
    MOCK_METHOD(void, onBegin, (int sampleRate, int sampleWidth, int channels), (override));
    MOCK_METHOD(void, onData, (const void* ptr, unsigned int count), (override));
    MOCK_METHOD(void, onEnd, (const SynthesisResult& result), (override));
};

/**
 * Following env variables must be set:
 *  - VOXER_MODEL_DIR
 *  - VOXER_FILES_DIR
 **/
TEST(TextToAudioTest, SynthesizeText)
{
    MockDataHandler handler;
    EXPECT_CALL(handler, onBegin(Gt(0), Gt(0), Gt(0))).Times(1);
    EXPECT_CALL(handler, onData(NotNull(), Gt(0))).Times(AtLeast(1));
    EXPECT_CALL(handler, onEnd).Times(1);

    EXPECT_NO_THROW({
        Voxer voxer;
        ASSERT_TRUE(voxer.configure());
        voxer.textToAudio("Hello world", handler);
        voxer.cleanup();
    });
}