/**
 * @brief C Color Library for Artifex
 *
 * @author SamTheDev
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Color Component Type
#define AX_COLOR_TYPE float

// RGBA Color
typedef struct {
    AX_COLOR_TYPE r, g, b, a;
} axColor;

// TODO advanced color functions

#ifdef __cplusplus
}
#endif