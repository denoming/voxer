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

#include "DataOutputStream.hpp"

namespace jar {

DataOutputStream::DataOutputStream(std::ostream& os)
    : _os{os}
{
}

void
DataOutputStream::onBegin(int /*sampleRate*/, int /*sampleWidth*/, int /*channels*/)
{
}

void
DataOutputStream::onData(const void* ptr, unsigned int size)
{
    _os.write(static_cast<const char*>(ptr), static_cast<std::streamsize>(size));
}

void
DataOutputStream::onEnd(const SynthesisResult& /*result*/)
{
}

} // namespace jar