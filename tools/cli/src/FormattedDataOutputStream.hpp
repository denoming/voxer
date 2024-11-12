// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#pragma once

#include "voxer/FormattedDataHandler.hpp"

#include <ostream>

namespace jar {

class FormattedDataOutputStream final : public FormattedDataHandler {
public:
    explicit FormattedDataOutputStream(DataFormat format, std::ostream& os);

    void
    onFormattedBegin(DataFormat format, int sampleRate, int sampleWidth, int channels) override;

    void
    onFormattedData(const void* ptr, size_t size) override;

    void
    onFormattedEnd(const SynthesisResult& result) override;

private:
    std::ostream& _os;
};

} // namespace jar