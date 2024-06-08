#pragma once

#include <cstdint>
#include <string>

// Null-UUID
#define nil 0

namespace Artifex {
typedef size_t ID;
typedef uint64_t id_t;

// Artifex UUID
struct UUID {
    uint16_t system;  // System Identificator
    uint64_t value;   // Unique Value (random)

    UUID(uint16_t systemID = 0);  // NOLINT(*-explicit-constructor)

    operator uint64_t() const;  // NOLINT(*-explicit-constructor)
};
}  // namespace Artifex
