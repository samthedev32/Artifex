#pragma once

#include <Artifex/types/shader.hpp>
#include <mathutil/common.hpp>

#include <cstdint>
#include <vector>

class Artifex;

// OpenGL Renderer
struct Render {
  public:
    // Initialize Renderer
    void init(Artifex *artifex);

    // DeInitialize Renderer
    void deinit();

    // Clear Screen
    void clear(float r, float g, float b);

    // Draw Line
    void line(vec2 a, vec2 b, vec3 color);

    // Draw Rect (Colored)
    void rect(vec2 center, vec2 size, vec3 color, float rotation = 0.0f);

    // Draw Rect (Texture)
    void rect(vec2 center, vec2 size, uint16_t tex, float rotation = 0.0f);

    // Draw Circle (Colored)
    void circle(vec2 center, float radius, vec3 color, float cutradius = 0.0f);

    // Draw Circle (Textured)
    void circle(vec2 center, float radius, uint16_t tex, float rotation = 0.0f,
                float cutradius = 0.0f, vec2 offset = 0.0f);

    // Draw Text
    void text(vec2 center, float width, vec3 color, float rotation = 0.0f);

  private:
    bool initialized = false;
    Artifex *ax = nullptr;

    GLuint VAO, VBO, EBO;
};