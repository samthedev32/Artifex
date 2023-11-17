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
  static void clear(vec<3> color);

  // Draw Rounded Rect (or even Sphere)
  void roundable(const vec<2> &center, const vec<2> &size, int look, const vec<3> &color, uint16_t tex, float corner = 0.0f,
                 float rotation = 0.0f);

  // Draw Single-Color Roundable-Rect
  //  void color(vec<2> center, vec<2> size, vec<3> color, float corner = 0.0f, float rotation = 0.0f);

  // Draw Dynamic-Color Roundable-Rect
  //  void dcolor(vec<2> center, vec<2> size, float corner = 0.0f, float rotation = 0.0f);

  // Draw Textured Roundable-Rect
  //  void texture(vec<2> center, vec<2> size, uint16_t texture, float corner = 0.0f, float rotation = 0.0f);

  // Draw Text
  void text(const vec<2> &center, float width, const vec<3> &color, float rotation = 0.0f);

private:
  bool initialized = false;
  Engine *engine = nullptr;

  GLuint VAO{}, VBO{}, EBO{};
};

} // namespace Artifex