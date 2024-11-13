// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#include "VoxerImpl.hpp"

#include "SynthesizerEngine.hpp"

#include <spdlog/spdlog.h>

namespace fs = std::filesystem;

static auto* kFilesDirName{"espeak-ng-data"};

namespace {

std::optional<std::string>
getEnv(const char* name)
{
    const char* ptr = std::getenv(name);
    return ptr ? std::string{ptr} : std::string{};
}

[[nodiscard]] bool
isFileExists(const fs::path& path)
{
    std::error_code ec{};
    if (const auto status = fs::status(path, ec); not ec) {
        return fs::exists(status) and fs::is_regular_file(status);
    }
    return false;
}

[[nodiscard]] bool
isDirExists(const fs::path& path)
{
    std::error_code ec{};
    if (const auto status = fs::status(path, ec); not ec) {
        return fs::exists(status) and fs::is_directory(status);
    }
    return false;
}

} // namespace

namespace jar {

bool
VoxerImpl::configure(const bool useCuda, const SpeakerId speakerId)
{
    const std::optional<fs::path> modelFile = getEnv("VOXER_MODEL_DIR");
    if (not modelFile) {
        SPDLOG_ERROR("Model file not set");
        return false;
    }

    if (not isFileExists(*modelFile)) {
        SPDLOG_ERROR("Model file <{}> does not exist",
                     modelFile->empty() ? "?" : modelFile->string());
        return false;
    }

    return configure(*modelFile, useCuda, speakerId);
}

bool
VoxerImpl::configure(const fs::path& modelPath, const bool useCuda, const SpeakerId speakerId)
{
    std::optional<fs::path> filesDir = getEnv("VOXER_FILES_DIR");
    if (not filesDir) {
        SPDLOG_DEBUG("Use <{}> relative path with data files", kFilesDirName);
        filesDir = fs::current_path() / kFilesDirName;
    }

    assert(filesDir);
    if (isDirExists(*filesDir)) {
        if (not configure(modelPath, *filesDir, useCuda, speakerId)) {
            SPDLOG_ERROR("Error on configure");
            return false;
        }
        return true;
    }

    return false;
}

bool
VoxerImpl::configure(const fs::path& modelPath,
                     const fs::path& filesPath,
                     bool useCuda,
                     SpeakerId speakerId)
{
    if (not SynthesizerEngine::setup(filesPath)) {
        SPDLOG_ERROR("Error on setup synthesizer");
        return false;
    }

    _model = std::make_unique<Model>(modelPath, useCuda, speakerId);
    _synthesizer = std::make_unique<Synthesizer>(*_model);

    return true;
}

void
VoxerImpl::cleanup()
{
    _model.reset();
    _synthesizer.reset();

    if (not SynthesizerEngine::cleanup()) {
        SPDLOG_ERROR("Error to cleanup synthesizer");
    }
}

SynthesisResult
VoxerImpl::textToAudio(std::string text, DataHandler& handler)
{
    return _synthesizer->synthesize(std::move(text), handler);
}

std::string
VoxerImpl::info() const
{
    const SynthesisConfig& sconfig = _model->synthesisConfig();
    std::stringstream ss;
    ss << fmt::format("{:>15} : {:<}\n", "Sample Rate", sconfig.sampleRate);
    ss << fmt::format("{:>15} : {:<} bits\n", "Sample Width", sconfig.sampleRate * 8);
    ss << fmt::format("{:>15} : {:<}\n", "Channels", sconfig.channels);
    ss << fmt::format("{:>15} : {:<.3f}\n", "Noise Scale", sconfig.noiseScale);
    ss << fmt::format("{:>15} : {:<.3f}\n", "Length Scale", sconfig.lengthScale);
    ss << fmt::format("{:>15} : {:<.3f}", "NoiseW", sconfig.noiseW);
    if (sconfig.speakerId) {
        ss << fmt::format("{:>15} : {:<}", "Speaker Id", static_cast<int>(*sconfig.speakerId));
    }
    return ss.str();
}

} // namespace jar
