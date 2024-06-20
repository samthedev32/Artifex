#pragma once

#include <stdlib.h>

//// ---- YUYV to RGB Implementation ---- ////

void yuyv_to_rgb_pixel(unsigned char* yuyv, unsigned char* rgb) {
    int y, v, u;
    float r, g, b;

    y = yuyv[0];  // y0
    u = yuyv[1];  // u0
    v = yuyv[3];  // v0

    r = y + 1.4065 * (v - 128);                       // r0
    g = y - 0.3455 * (u - 128) - 0.7169 * (v - 128);  // g0
    b = y + 1.1790 * (u - 128);                       // b0

    if (r < 0)
        r = 0;
    else if (r > 255)
        r = 255;
    if (g < 0)
        g = 0;
    else if (g > 255)
        g = 255;
    if (b < 0)
        b = 0;
    else if (b > 255)
        b = 255;

    rgb[0] = (unsigned char)r;
    rgb[1] = (unsigned char)g;
    rgb[2] = (unsigned char)b;

    // second pixel
    u = yuyv[1];  // u0
    y = yuyv[2];  // y1
    v = yuyv[3];  // v0

    r = y + 1.4065 * (v - 128);                       // r1
    g = y - 0.3455 * (u - 128) - 0.7169 * (v - 128);  // g1
    b = y + 1.1790 * (u - 128);                       // b1

    if (r < 0)
        r = 0;
    else if (r > 255)
        r = 255;
    if (g < 0)
        g = 0;
    else if (g > 255)
        g = 255;
    if (b < 0)
        b = 0;
    else if (b > 255)
        b = 255;

    rgb[3] = (unsigned char)r;
    rgb[4] = (unsigned char)g;
    rgb[5] = (unsigned char)b;
}

int yuyv_to_rgb(unsigned char* yuyv, unsigned char* rgb, int width,
                int height) {
    unsigned char temp = 0;
    long yuv_size = height * width * 2;
    long rgb_size = height * width * 3;

    if (yuyv == NULL || rgb == NULL)
        return 0;

    for (int i = 0, j = 0; i < rgb_size && j < yuv_size; i += 6, j += 4) {
        yuyv_to_rgb_pixel(&yuyv[j], &rgb[i]);
    }
    return 1;
}