// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#include "voxer/Voxer.hpp"
#include "voxer/Version.hpp"

#include "VoxerImpl.hpp"

#include <fmt/format.h>

namespace fs = std::filesystem;

namespace jar {

[[nodiscard]] std::string
Voxer::version()
{
    return fmt::format("Voxer {} ({})", getVersion(), getVersionRevision());
}

Voxer::Voxer()
    : _impl{new VoxerImpl}
{
}

Voxer::~Voxer() = default;

bool
Voxer::configure(const bool useCuda, const SpeakerId speakerId)
{
    return _impl->configure(useCuda, speakerId);
}

bool
Voxer::configure(const fs::path& modelPath, const bool useCuda, const SpeakerId speakerId)
{
    return _impl->configure(modelPath, useCuda, speakerId);
}

bool
Voxer::configure(const fs::path& modelPath,
                 const fs::path& filesPath,
                 const bool useCuda,
                 const SpeakerId speakerId)
{
    return _impl->configure(modelPath, filesPath, useCuda, speakerId);
}

void
Voxer::cleanup()
{
    return _impl->cleanup();
}

SynthesisResult
Voxer::textToAudio(std::string text, DataHandler& handler)
{
    return _impl->textToAudio(std::move(text), handler);
}

std::string
Voxer::info() const
{
    return _impl->info();
}

} // namespace jar
