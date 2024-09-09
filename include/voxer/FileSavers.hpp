// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#pragma once

#include "voxer/VoxerExport.hpp"
#include "voxer/FileSaver.hpp"

#include <memory>

namespace jar {

VOXER_EXPORT std::unique_ptr<FileSaver>
getWavFileSaver();

}