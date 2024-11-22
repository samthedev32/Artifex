/**
 * @brief C Callback Function Definitions for Artifex
 *
 * @author SamTheDev
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// Struct Passed to every module callback
struct axModuleInfo {
    uint64_t id;  // module id

    // Timing
    double delta;  // deltaTime
    // uint64_t tick;  // system tick

    // Pointer to the Engine (same as `Artifex`)
    struct _Artifex* engine;
};

// uint64_t module_id, void* user_ptr;

typedef void (*_ax_fn_create_t)(const struct axModuleInfo*, void*);
typedef void (*_ax_fn_destroy_t)(const struct axModuleInfo*, void*);
typedef void (*_ax_fn_update_t)(const struct axModuleInfo*, void*);

#ifdef __cplusplus
}
#endif