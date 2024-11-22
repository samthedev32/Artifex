/**
 * @brief Artifex Engine (C API)
 *
 * @author SamTheDev
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <Artifex/core/types/id.h>
#include <Artifex/fundef.h>

// Engine
typedef struct _Artifex* Artifex;

// Module Descriptor
struct axModuleDescriptor {
    // User Data
    void* user;  // pass NULL to make it entity-only

    // Callback Function Pointers
    _ax_fn_create_t onCreate;
    _ax_fn_destroy_t onDestroy;
    _ax_fn_update_t onUpdate;

    // Dependencies
    // id_t* dependencies;  // a value of 0 will be the whole update loop (for rendering, etc)
    // a module is quaranteed to be on the same thread as (and called after) its dependencies

    // Config
    // int synced;  // wait for it to finish before updating or not?
};

// ---- Basic Control

// Initialize Engine
int axInitialize(Artifex* ax, uint8_t threads);

// Destroy Engine
void axDestroy(Artifex* ax);

// Run Update (every frame)
int axUpdate(Artifex ax);

// or

// Start Game Loop
int axStartLoop(Artifex ax);

// ---- Management

// Register Module
id_t axRegister(Artifex ax, const struct axModuleDescriptor* descriptor);

// // Enable Module
// int axEnable(Artifex ax, id_t moduleID);

// // Disable Module
// int axDisable(Artifex ax, id_t moduleID);

// ---- ECS

// id_t axEntityAdd();

#ifdef __cplusplus
}
#endif
