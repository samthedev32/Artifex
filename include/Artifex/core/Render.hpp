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

    // Draw Rounded Rect
    void rounded(vec<2> center, vec<2> size, uint16_t tex, float amount,
                 float rotation = 0.0f);

    // Draw Text
    void text(vec<2> center, float width, vec<3> color, float rotation = 0.0f);

    // TODO: Rotations

  private:
    bool initialized = false;
    Engine *engine = nullptr;

    GLuint VAO, VBO, EBO;
};

} // namespace Artifex