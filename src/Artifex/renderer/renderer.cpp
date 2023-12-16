#include "Artifex/core/renderer.hpp"

#include <cstring>

// Radian-Degree Conversion Functions
#define rads(d) (d * M_PI / 180.0f)
#define degs(r) (r * 180.0f / M_PI)

namespace Artifex {

Renderer::Renderer(const std::string &name, const vec<2, uint32_t> &size) : Window(name, size), load(*this), draw(*this) {
  // Load Default Rect
  vec<2, float> vertices[] = {
      // positions      // texture coords
      {-1.0f, 1.0f},  {0.0f, 1.0f}, // top right
      {-1.0f, -1.0f}, {0.0f, 0.0f}, // bottom right
      {1.0f, -1.0f},  {1.0f, 0.0f}, // bottom left
      {1.0f, 1.0f},   {1.0f, 1.0f}  // top left
  };

  uint32_t indices[] = {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };

  load.mesh(vertices, sizeof(vertices), indices, sizeof(indices));

  // Load 2D Shader
  // TODO: default shader
  // engine->load.shader(default_shader_code[0],
  // default_shader_code[1]);
  //  engine->load.shader("../../../examples/hello_world/shader/2d.glsl");

  // Load Default Texture
  unsigned char data[] = {255};
  load.texture(data, {1}, 1);
}

Renderer::~Renderer() {
  // Delete Buffers
  for (auto m : meshes)
    glDeleteBuffers(3, &m.VAO);
  meshes.clear();
}

void Renderer::clear(vec<3> color) {
  // Clear Screen
  glClearColor(color->red, color->green, color->blue, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

} // namespace Artifex