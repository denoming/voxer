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

#include <string>

namespace jar {

class Model;
class DataHandler;

class Synthesizer {
public:
    explicit Synthesizer(Model& model);

    [[maybe_unused]] SynthesisResult
    synthesize(std::string text, DataHandler& handler);

private:
    [[maybe_unused]] SynthesisResult
    synthesize(PhonemeIds& phonemeIds, AudioBuffer& audioBuffer) const;

private:
    Model& _model;
};

} // namespace jar