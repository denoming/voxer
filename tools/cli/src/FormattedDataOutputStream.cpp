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

#include "FormattedDataOutputStream.hpp"

namespace jar {

FormattedDataOutputStream::FormattedDataOutputStream(DataFormat format, std::ostream& os)
    : FormattedDataHandler{format}
    , _os{os}
{
}

void
FormattedDataOutputStream::onFormattedBegin(DataFormat /*format*/,
                                            int /*sampleRate*/,
                                            int /*sampleWidth*/,
                                            int /*channels*/)
{
}

void
FormattedDataOutputStream::onFormattedData(const void* ptr, size_t size)
{
    _os.write(static_cast<const char*>(ptr), static_cast<std::streamsize>(size));
}

void
FormattedDataOutputStream::onFormattedEnd(const SynthesisResult& result)
{
    _os.flush();
}

} // namespace jar