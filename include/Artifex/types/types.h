#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <string>
#include <map>

// Include OpenGL if needed
#ifndef __gl_h_
#include <GL/glad.h>
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

// C types
typedef unsigned long int ulong;
typedef unsigned short int ushort;
typedef unsigned int uint;

// Radian-Degree Conversion Functions
#define rads(d) (d * M_PI / 180.0f) //< convert degrees to radians
#define degs(r) (r * 180.0f / M_PI) //< convert radians to degrees

// Include Types
#include <artifex/types/vector.h>
#include <artifex/types/matrix.h>
#include <artifex/types/shader.h>

// OpenGL Texture ID
typedef uint texture;

// Text Font
struct font
{
    texture data; //< Font Data
    int width, height;
    float start;

    vec2 VERT[6];
    vec2 UV[6];

    uint VAO, VBO; //< Rendering Data

    float vertices[24] = {
        // positions      // texture coords
        -1.0f, 1.0f, 0.0f, 1.0f,  // 0 top right
        -1.0f, -1.0f, 0.0f, 0.0f, // 1 bottom right
        1.0f, -1.0f, 1.0f, 0.0f,  // 3 top left

        -1.0f, 1.0f, 0.0f, 1.0f, // 1 bottom right
        1.0f, -1.0f, 1.0f, 0.0f, // 2 bottom left
        1.0f, 1.0f, 1.0f, 1.0f,  // 3 top left
    };
};

// Audio Source
struct audio
{
    Mix_Chunk *source = NULL;
    int ch = -1;
};