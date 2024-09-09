#include "voxer/FileSavers.hpp"

#include "WavFileSaver.hpp"

namespace jar {

std::unique_ptr<FileSaver>
getWavFileSaver()
{
    return std::make_unique<WavFileSaver>();
}

} // namespace jar