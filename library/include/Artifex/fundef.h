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

// uint64_t module_id, void* user_ptr;

typedef void (*_ax_fn_create_t)(uint64_t, void*);
typedef void (*_ax_fn_destroy_t)(uint64_t, void*);
typedef void (*_ax_fn_update_t)(uint64_t, void*);

#ifdef __cplusplus
}
#endif