#include "voxer/Voxer.hpp"

#include "VoxerImpl.hpp"

#include <fmt/std.h>

#include <filesystem>

namespace fs = std::filesystem;

namespace jar {
Voxer::Voxer()
    : _impl{new VoxerImpl}
{
}

Voxer::~Voxer() = default;

void
Voxer::configure(const std::string& modelPath, const bool useCuda, const SpeakerId speakerId)
{
    if (const auto filesPath = fs::current_path() / "files"; fs::exists(filesPath)) {
        configure(filesPath, modelPath, useCuda, speakerId);
    } else {
        throw std::runtime_error{fmt::format("Data path not found: {}", filesPath)};
    }
}

void
Voxer::configure(const std::string& filesPath,
                 const std::string& modelPath,
                 const bool useCuda,
                 const SpeakerId speakerId)
{
    return _impl->configure(filesPath, modelPath, useCuda, speakerId);
}

void
Voxer::cleanup()
{
    return _impl->cleanup();
}

SynthesisResult
Voxer::textToAudio(std::string text, std::vector<int16_t>& buffer, const Callback& callback)
{
    return _impl->textToAudio(std::move(text), buffer, callback);
}

} // namespace jar
