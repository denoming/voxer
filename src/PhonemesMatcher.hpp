// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#pragma once

#include "Types.hpp"

namespace jar {

class PhonemesMatcher {
public:
    PhonemesMatcher() = default;

    explicit PhonemesMatcher(const Phonemes& phonemes, const PhonemeIdConfig& config);

    void
    match(const Phonemes& phonemes, const PhonemeIdConfig& config);

    [[nodiscard]] PhonemeIds::const_iterator
    begin() const;

    [[nodiscard]] PhonemeIds::const_iterator
    end() const;

    [[nodiscard]] bool
    empty() const;

    [[nodiscard]] PhonemeIds
    release();

private:
    PhonemeIds _ids;
};

} // namespace jar