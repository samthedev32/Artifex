/**
 * @brief C 3 Channel Color Library for Artifex
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
// https://en.wikipedia.org/wiki/Y%E2%80%B2UV
// https://en.wikipedia.org/wiki/HSL_and_HSV

// 3 Channel Color (RGB, YUV, HSV)
typedef struct {
    union {
        struct {
            AX_COLOR_TYPE r, g, b;
        };

        struct {
            AX_COLOR_TYPE y, u, v;
        };

        struct {
            AX_COLOR_TYPE h, s;  // v is @same place as YU`V`
        };

        // Individual Component Data
        AX_COLOR_TYPE data[3];
    };
} color3;

color3 color3_rgb_to_yuv(color3 rgb);
color3 color3_rgb_to_hsv(color3 rgb);

color3 color3_yuv_to_rgb(color3 yuv);
color3 color3_yuv_to_hsv(color3 yuv);

color3 color3_hsv_to_rgb(color3 hsv);
color3 color3_hsv_to_yuv(color3 hsv);

#ifdef __cplusplus
}
#endif