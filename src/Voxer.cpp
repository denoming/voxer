// Copyright 2025 Denys Asauliak
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
