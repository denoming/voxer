// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <voxer/Voxer.hpp>
#include <voxer/FormattedDataHandler.hpp>

#include <gst/gst.h>
#include <gst/app/gstappsrc.h>

#include <thread>

using namespace jar;
using namespace testing;

static auto kModelPath{"voxer/voices/en_GB-alba-medium.onnx"};
static auto kFilesPath{"espeak-ng-data"};

static const char* kPipelineDescription = R"(
    appsrc name=src ! wavparse ! audioconvert ! audioresample ! autoaudiosink
)";

class TextToPlaybackTest : public FormattedDataHandler, public Test {
public:
    static void
    SetUpTestCase()
    {
        gst_init(nullptr, nullptr);
    }

    TextToPlaybackTest()
        : FormattedDataHandler{DataFormat::Wav}
    {
    }

    void
    createPipeline();

    void
    destroyPipeline() const;

    void
    startPipeline() const;

    void
    stopPipeline() const;

    void
    waitPipeline() const;

    void
    playAudio(const void* ptr, size_t size) const;

private:
    void
    onFormattedBegin(const DataFormat format,
                     const int sampleRate,
                     const int sampleWidth,
                     const int channels) override
    {
        g_message("onFormattedBegin: rate<%d>, width<%d>, channels<%d>",
                  (format == DataFormat::Wav) ? "Wav" : "Other",
                  sampleRate,
                  sampleWidth,
                  channels);
    }

    void
    onFormattedData(const void* ptr, size_t size) override
    {
        g_message("onFormattedData");
        playAudio(ptr, size);
    }

    void
    onFormattedEnd(const SynthesisResult& result) override
    {
        g_message("onFormattedDone");
        stopPipeline();
    }

private:
    GstElement* _pipeline{};
    GstElement* _src{};
};

void
TextToPlaybackTest::createPipeline()
{
    _pipeline = gst_parse_launch(kPipelineDescription, nullptr);
    ASSERT_THAT(_pipeline, NotNull());
    _src = gst_bin_get_by_name(GST_BIN(_pipeline), "src");
    ASSERT_THAT(_src, NotNull());
}

void
TextToPlaybackTest::startPipeline() const
{
    GstStateChangeReturn retVal{};
    retVal = gst_element_set_state(_pipeline, GST_STATE_PLAYING);
    g_message("Set state to playing: %d", retVal);
}

void
TextToPlaybackTest::stopPipeline() const
{
    gst_app_src_end_of_stream(GST_APP_SRC(_src));
}

void
TextToPlaybackTest::waitPipeline() const
{
    /* Wait until error or EOS */
    const auto types = static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS);
    GstBus* bus = gst_element_get_bus(_pipeline);
    GstMessage* msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, types);
    /* See next tutorial for proper error message handling/parsing */
    if (GST_MESSAGE_TYPE(msg) == GST_MESSAGE_ERROR) {
        g_printerr("An error occurred! Set GST_DEBUG=*:WARN env to get more details\n");
    }

    /* Free resources */
    gst_message_unref(msg);
    gst_object_unref(bus);
}

void
TextToPlaybackTest::playAudio(const void* ptr, const size_t size) const
{
    auto* buffer = gst_buffer_new_allocate(nullptr, size, nullptr);
    ASSERT_THAT(buffer, NotNull());

    GstMapInfo info;
    if (gst_buffer_map(buffer, &info, GST_MAP_WRITE) == TRUE) {
        memcpy(info.data, ptr, size);
        gst_buffer_unmap(buffer, &info);
    }

    GstFlowReturn ret{GST_FLOW_OK};
    g_signal_emit_by_name(_src, "push-buffer", buffer, &ret);
    gst_buffer_unref(buffer);
}

void
TextToPlaybackTest::destroyPipeline() const
{
    GstStateChangeReturn retVal{};
    retVal = gst_element_set_state(_pipeline, GST_STATE_NULL);
    if (retVal == GST_STATE_CHANGE_ASYNC) {
        /* Waiting async transition to complete */
        GstState state{}, pending{};
        retVal = gst_element_get_state(_pipeline, &state, &pending, GST_CLOCK_TIME_NONE);
        EXPECT_EQ(retVal, GST_STATE_CHANGE_SUCCESS);
    }

    gst_object_unref(_src);
    gst_object_unref(_pipeline);
}

TEST_F(TextToPlaybackTest, SynthesizeTextAndPlayback)
{
    createPipeline();
    startPipeline();

    Voxer voxer;
    EXPECT_NO_THROW(voxer.configure(kFilesPath, kModelPath, false, SpeakerId::Default));
    voxer.textToAudio("Hello world", *this);
    EXPECT_NO_THROW(voxer.cleanup());

    waitPipeline();
    destroyPipeline();
}