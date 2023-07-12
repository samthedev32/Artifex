#pragma once

#include <map>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

// C types
typedef unsigned long int ulong;
typedef unsigned short int ushort;
typedef unsigned int uint;

// Radian-Degree Conversion Functions
#define rads(d) (d * M_PI / 180.0f) //< convert degrees to radians
#define degs(r) (r * 180.0f / M_PI) //< convert radians to degrees

// Include Types
#include <Artifex/types/shader.hpp>

// OpenGL Texture ID
typedef uint texture;

// Text Font
struct font {
    texture data; //< Font Data
    int width, height;
    float start;

    vec2 VERT[6];
    vec2 UV[6];

    uint VAO, VBO; //< Rendering Data

    float vertices[24] = {
        // positions      // texture coords
        -1.0f, 1.0f,  0.0f, 1.0f, // 0 top right
        -1.0f, -1.0f, 0.0f, 0.0f, // 1 bottom right
        1.0f,  -1.0f, 1.0f, 0.0f, // 3 top left

        -1.0f, 1.0f,  0.0f, 1.0f, // 1 bottom right
        1.0f,  -1.0f, 1.0f, 0.0f, // 2 bottom left
        1.0f,  1.0f,  1.0f, 1.0f, // 3 top left
    };
};