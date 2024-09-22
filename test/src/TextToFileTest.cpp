// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <voxer/Voxer.hpp>
#include <voxer/FormattedDataSaver.hpp>

#include <filesystem>

namespace fs = std::filesystem;

static auto kModelPath{"voxer/voices/en_GB-alba-medium.onnx"};
static auto kFilesPath{"espeak-ng-data"};
static const char* kOutputFile{"Speech.wav"};

using namespace jar;
using namespace testing;

TEST(TextToFileTest, SynthesizeTextWithSaving)
{
    ASSERT_TRUE(fs::exists(kModelPath));

    FormattedDataSaver handler{DataFormat::Wav, kOutputFile};

    EXPECT_NO_THROW({
        Voxer voxer;
        voxer.configure(kFilesPath, kModelPath, false, SpeakerId::Default);
        voxer.textToAudio("Hello world", handler);
        voxer.cleanup();
    });

    EXPECT_THAT(fs::file_size(kOutputFile), Gt(0));
    fs::remove(kOutputFile);
}
