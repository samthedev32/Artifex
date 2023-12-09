#pragma once

#include <Artifex/core/window.hpp>
#include <Artifex/types/shader.hpp>

#include <cstdint>
#include <vector>

namespace Artifex {

class Engine;

// OpenGL Renderer
class Renderer : public Window {
public:
  Renderer(const std::string &name, const vec<2, uint32_t> &size);
  ~Renderer();

  // Clear Screen
  static void clear(vec<3> color);

  // Load Shader
  size_t shader(const char *vertex, const char *fragment, const char *geometry = nullptr);

  // Load Texture
  size_t texture(void *data, const vec<2, uint32_t> &size, uint8_t channels);

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
  //  void text(const vec<2> &center, float width, const vec<3> &color, float rotation = 0.0f);

private:
  GLuint VAO{}, VBO{}, EBO{};

  std::vector<Shader> shaders;
  std::vector<GLuint> textures;
};

} // namespace Artifex