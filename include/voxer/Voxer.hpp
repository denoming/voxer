#pragma once

#include "VoxerExport.hpp"

#include <memory>

namespace jar {

class VOXER_EXPORT Voxer {
public:
    Voxer();

    ~Voxer();

private:
    std::unique_ptr<class VoxerImpl> _impl;
};

} // namespace My
