#include <utility>
#include <Artifex/types/uuid.hpp>

namespace Artifex
{
    UUID::UUID(uint16_t systemID)
    {
        system = systemID;
        value = rand() % UINT64_MAX;
    }


    UUID::operator uint64_t() const
    {
        return value;
    }
}
