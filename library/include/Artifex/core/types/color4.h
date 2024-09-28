/**
 * @brief C 4 Channel Color Library for Artifex
 *
 * @author SamTheDev
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Color Component Type
#ifndef AX_COLOR_TYPE
#define AX_COLOR_TYPE float
#endif

// TODO see:
// https://en.wikipedia.org/wiki/CMYK_color_model

// 4 Channel Color (RGBA, CMYK)
typedef struct {
    union {
        struct {
            AX_COLOR_TYPE r, g, b, a;
        };

        struct {
            AX_COLOR_TYPE c, m, y, k;
        };

        // Individual Component Data
        AX_COLOR_TYPE data[4];
    };
} color4;

color4 color4_rgba_to_cmyk(color4 rgba);
color4 color4_cmyk_to_rgba(color4 cmyk);

#ifdef __cplusplus
}
#endif