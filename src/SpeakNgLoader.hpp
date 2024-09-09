// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#pragma once

#include <string>

namespace jar {

class SpeakNgLoader {
public:
    explicit SpeakNgLoader(const std::string& filesPath);

    ~SpeakNgLoader();
};

} // namespace jar