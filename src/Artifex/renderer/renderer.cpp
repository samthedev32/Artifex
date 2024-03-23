#include <Artifex/core/Renderer.hpp>

#define GLAD_IMPLEMENTATION
#include "GL/glad.h"

#include <cstring>

// Radian-Degree Conversion Functions
#define rads(d) ((d) * M_PI / 180.0f)
#define degs(r) ((r) * 180.0f / M_PI)

namespace Artifex {

extern const char *_shader[2];

Renderer::Renderer(const std::string &name, const vec<2, uint32_t> &size) : Window(name, size) {
  // Load Default Shader
  // base.shader = load_shader(_shader[0], _shader[1]);
  base.shader = load_shader("../../../examples/hello_world/shaders/2d.glsl");

  // Load Default Mesh
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

  base.mesh = load_mesh(vertices, sizeof(vertices) * sizeof(*vertices), indices, sizeof(indices) * sizeof(*indices));

  // Load Default Texture
  unsigned char data[] = {255};
  base.texture = load_texture(data, {1}, 1);
}

Renderer::~Renderer() {
  // Delete Textures
  for (auto [_, t] : textures)
    glDeleteTextures(1, &t);
  textures.clear();

  // Delete Buffers
  for (auto [_, m] : meshes)
    glDeleteBuffers(3, &m.VAO);
  meshes.clear();

  // Delete Shaders
  for (const auto &[_, s] : shaders)
    glDeleteProgram(s.id);
  shaders.clear();
}

void Renderer::clear(vec<3> color) {
  // Clear Screen
  glClearColor(color->red, color->green, color->blue, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

const char *_shader[2] = {
    // Vertex Shader
    "#version 300 es\n"

    "layout(location = 0) in vec2 aPos;\n"
    "layout(location = 1) in vec2 aTexCoord;\n"

    "void main() {\n"
    "    gl_Position = vec4(aPos.xy, 0.0, 0.0);\n"
    "}\n",

    // Fragment Shader
    "#version 300 es\n"

    "precision mediump float;\n"
    "out vec4 FragColor;\n"

    "void main() {\n"
    "    FragColor = vec4(1.0, 0.4, 0.0, 1.0);\n"
    "}\n"};

} // namespace Artifex