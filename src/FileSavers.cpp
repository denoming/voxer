// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#include "voxer/FileSavers.hpp"

#include "WavFileSaver.hpp"

namespace jar {

std::unique_ptr<FileSaver>
getWavFileSaver()
{
    return std::make_unique<WavFileSaver>();
}

} // namespace jar