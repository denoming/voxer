// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <voxer/Voxer.hpp>
#include <voxer/FileSavers.hpp>

#include <filesystem>

namespace fs = std::filesystem;

static const char* kModelPath{"voices/en_GB-alba-medium.onnx"};
static const char* kOutputFile{"SavingToFile.wav"};

using namespace jar;
using namespace testing;

TEST(Voxer, TextToFile)
{
    ASSERT_TRUE(fs::exists(kModelPath));

    Voxer voxer;
    EXPECT_NO_THROW(voxer.configure(kModelPath));

    auto fileSaver = getWavFileSaver();
    ASSERT_TRUE(fileSaver);

    voxer.onPreroll([&fileSaver](const int sampleRate, const int sampleSize, const int channels) {
        // Open file for writing
        EXPECT_NO_THROW(fileSaver->open(kOutputFile, sampleRate, sampleSize, channels));
    });
    voxer.onBuffer([&fileSaver](const AudioBuffer& buffer) {
        // Save audio buffer to a file
        EXPECT_NO_THROW(fileSaver->write(buffer));
    });

    {
        std::vector<int16_t> audioBuffer;
        EXPECT_NO_THROW(voxer.textToAudio("Hello world", audioBuffer));
        EXPECT_THAT(audioBuffer, IsEmpty());
    }

    EXPECT_NO_THROW(voxer.cleanup());

    EXPECT_THAT(fs::file_size(kOutputFile), Gt(0));
    fs::remove(kOutputFile);
}
