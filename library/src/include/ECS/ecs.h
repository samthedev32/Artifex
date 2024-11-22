/**
 * @brief Artifex's Entity Component System
 *
 * @author SamTheDev
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

struct _ax_ecs {
    // Values
    void* values[256 + 8];  // data (starting with u64 as length)
    uint16_t sizes[256];    // type sizes

    // Archetypes
    // TODO
} ecs;

#ifdef __cplusplus
}
#endif