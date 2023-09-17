#pragma once

#include <Artifex/types/shader.hpp>

#include <cstdint>
#include <vector>

namespace Artifex {

class Engine;

// OpenGL Renderer
struct Render {
  public:
    // Initialize Renderer
    void init(Engine *pEngine);

    // DeInitialize Renderer
    void deinit();

    // Clear Screen
    void clear(vec<3> color);

    // Draw Line
    void line(vec<2> a, vec<2> b, vec<3> color);

    // Draw Rect (Colored)
    void rect(vec<2> center, vec<2> size, vec<3> color, float rotation = 0.0f);

    // Draw Rect (Texture)
    void rect(vec<2> center, vec<2> size, uint16_t tex, float rotation = 0.0f);

    // Draw Circle (Colored)
    void circle(vec<2> center, float radius, vec<3> color,
                float cutradius = 0.0f);

    // Draw Circle (Textured)
    void circle(vec<2> center, float radius, uint16_t tex,
                float rotation = 0.0f, float cutradius = 0.0f,
                vec<2> offset = 0.0f);

    // Draw Rounded Corner
    void rounded(vec<2> center, float radius, uint16_t tex, float amount);

    // Draw Text
    void text(vec<2> center, float width, vec<3> color, float rotation = 0.0f);

    // TODO: Rotations

  private:
    bool initialized = false;
    Engine *engine = nullptr;

    GLuint VAO, VBO, EBO;
};

} // namespace Artifex