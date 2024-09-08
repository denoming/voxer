#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "voxer/Voxer.hpp"

static const std::string kModelPath{"voices/en_GB-alba-medium.onnx"};

using namespace testing;

TEST(Voxer, Dummy)
{
    jar::Voxer voxer;
    voxer.configure(kModelPath);
    {
        std::vector<int16_t> audioBuffer;
        voxer.textToAudio("Hello World", audioBuffer, nullptr);
        ASSERT_THAT(audioBuffer, Not(IsEmpty()));
    }
    voxer.cleanup();
}
