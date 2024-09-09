#pragma once

#include "voxer/Types.hpp"

#include <filesystem>
#include <memory>

namespace jar {

class FileSaver {
public:
    FileSaver() = default;

    virtual ~FileSaver() = default;

    [[nodiscard]] virtual bool
    isOpened() const
        = 0;

    virtual void
    open(const std::filesystem::path& outputFile, int sampleRate, int sampleSize, int channels)
        = 0;

    virtual void
    write(const AudioBuffer& buffer)
        = 0;

    virtual void
    close()
        = 0;
};

} // namespace jar