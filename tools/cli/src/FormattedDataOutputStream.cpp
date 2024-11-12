// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

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