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

static const auto* kAudioFile{"output.wav"};

using namespace jar;
using namespace testing;

/**
 * Following env variables must be set:
 *  - VOXER_MODEL_DIR
 *  - VOXER_FILES_DIR
 **/
TEST(TextToFileTest, SynthesizeTextWithSaving)
{
    FormattedDataSaver handler{DataFormat::Wav, kAudioFile};

    EXPECT_NO_THROW({
        Voxer voxer;
        ASSERT_TRUE(voxer.configure());
        voxer.textToAudio("Hello world", handler);
        voxer.cleanup();
    });

    EXPECT_THAT(fs::file_size(kAudioFile), Gt(0));
    fs::remove(kAudioFile);
}
