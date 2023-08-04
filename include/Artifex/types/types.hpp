#pragma once

#include <map>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include <Artifex/types/shader.hpp>

namespace Artifex {

// OpenGL Texture ID
typedef GLuint Texture;

// Text Font
struct Font {
    Texture data; //< Font Data
    uint8_t width, height;
    float start;

    vec2 VERT[6];
    vec2 UV[6];

    GLuint VAO, VBO; //< Rendering Data

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

} // namespace Artifex