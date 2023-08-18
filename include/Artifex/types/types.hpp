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
struct Texture {
    int width, height, channels;

    GLuint id;
};

// Text Font
struct Font {
    Texture data;
    uint8_t rows, cols;

    vec2 VERT[6];
    vec2 UV[6];

    GLuint VAO, VBO;

    const float vertices[24] = {
        -1.0f, 1.0f,  0.0f, 1.0f, // 0 top right
        -1.0f, -1.0f, 0.0f, 0.0f, // 1 bottom right
        1.0f,  -1.0f, 1.0f, 0.0f, // 3 top left

        -1.0f, 1.0f,  0.0f, 1.0f, // 1 bottom right
        1.0f,  -1.0f, 1.0f, 0.0f, // 2 bottom left
        1.0f,  1.0f,  1.0f, 1.0f, // 3 top left
    };
};

} // namespace Artifex