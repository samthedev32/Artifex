/**
 * @brief Artifex Engine (C API)
 *
 * @author SamTheDev
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _Artifex* Artifex;

// Initialize Engine
int axInitialize(Artifex* ax);

// Destroy Engine
void axDestroy(Artifex* ax);

// Run Update (every frame)
int axUpdate(Artifex* ax);

// or

// Start Game Loop
void axStartLoop(Artifex* ax);

#ifdef __cplusplus
}
#endif