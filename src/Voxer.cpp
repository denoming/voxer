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

void
Voxer::onPreroll(PrerollCallback callback)
{
    return _impl->onPreroll(std::move(callback));
}

void
Voxer::onBuffer(BufferCallback callback)
{
    return _impl->onBuffer(std::move(callback));
}

SynthesisResult
Voxer::textToAudio(std::string text, AudioBuffer& buffer)
{
    return _impl->textToAudio(std::move(text), buffer);
}

} // namespace jar
