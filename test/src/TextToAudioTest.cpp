#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <voxer/Voxer.hpp>

static const char* kModelPath{"voices/en_GB-alba-medium.onnx"};

using namespace jar;
using namespace testing;

TEST(Voxer, TextToAudio)
{
    Voxer voxer;
    EXPECT_NO_THROW(voxer.configure(kModelPath));

    {
        std::vector<int16_t> audioBuffer;
        EXPECT_NO_THROW(voxer.textToAudio("Hello world", audioBuffer));
        EXPECT_THAT(audioBuffer, Not(IsEmpty()));
    }

    EXPECT_NO_THROW(voxer.cleanup());
}