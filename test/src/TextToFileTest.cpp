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

static const char* kModelPath{"voices/en_GB-alba-medium.onnx"};
static const char* kOutputFile{"SavingToFile.wav"};

using namespace jar;
using namespace testing;

TEST(Voxer, TextToFile)
{
    ASSERT_TRUE(fs::exists(kModelPath));

    FormattedDataSaver handler{DataFormat::Wav, kOutputFile};

    EXPECT_NO_THROW({
        Voxer voxer;
        voxer.configure(kModelPath);
        voxer.textToAudio("Hello world", handler);
        voxer.cleanup();
    });

    EXPECT_THAT(fs::file_size(kOutputFile), Gt(0));
    fs::remove(kOutputFile);
}
