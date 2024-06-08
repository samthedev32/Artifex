#pragma once

namespace Artifex::RAW
{
    const vec<2> vertices[] = {
        // positions      // texture coords
        {-1.0f, 1.0f}, {0.0f, 1.0f}, // top right
        {-1.0f, -1.0f}, {0.0f, 0.0f}, // bottom right
        {1.0f, -1.0f}, {1.0f, 0.0f}, // bottom left
        {1.0f, 1.0f}, {1.0f, 1.0f} // top left
    };

    const uint32_t indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3 // second triangle
    };
}
