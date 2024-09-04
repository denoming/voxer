#include "voxer/Voxer.hpp"

#include "VoxerImpl.hpp"

namespace jar {

Voxer::Voxer()
    : _impl{new VoxerImpl}
{
}

} // namespace jar
