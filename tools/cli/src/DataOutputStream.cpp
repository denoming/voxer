// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

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