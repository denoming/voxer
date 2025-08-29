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