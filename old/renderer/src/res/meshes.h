#pragma once

#include <Artifex/vec2.h>

const vec2 res_vertices[] = {
    // positions      // texture coords
    {-1.0f, 1.0f},
    {0.0f, 0.0f},  // top right
    {-1.0f, -1.0f},
    {0.0f, 1.0f},  // bottom right
    {1.0f, -1.0f},
    {1.0f, 1.0f},  // bottom left
    {1.0f, 1.0f},
    {1.0f, 0.0f}  // top left
};

const unsigned int res_indices[] = {
    0, 1, 3,  // first triangle
    1, 2, 3   // second triangle
};