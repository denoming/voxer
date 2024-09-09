#pragma once

#include "voxer/VoxerExport.hpp"
#include "voxer/FileSaver.hpp"

#include <memory>

namespace jar {

VOXER_EXPORT std::unique_ptr<FileSaver>
getWavFileSaver();

}