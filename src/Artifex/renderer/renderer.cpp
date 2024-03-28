#include <Artifex/core/Renderer.hpp>

#define GLAD_IMPLEMENTATION
#include "GL/glad.h"

#include <cstring>

// Radian-Degree Conversion Functions
#define rads(d) ((d) * M_PI / 180.0f)
#define degs(r) ((r) * 180.0f / M_PI)

namespace Artifex {

extern const char *_shader[2];

Renderer::Renderer(const std::string &name, const vec<2, uint32_t> &size) : window(name, size) {
  // Load Default Shader
  // base.shader = load_shader(_shader[0], _shader[1]);
  base.shader = load_shader("../../../examples/gfx_simple/shader/2d.glsl");

  // Load Default Mesh
  const vec<2, float> vertices[] = {
      // positions      // texture coords
      {-1.0f, 1.0f},  {0.0f, 0.0f}, // top right
      {-1.0f, -1.0f}, {1.0f, 0.0f}, // bottom right
      {1.0f, -1.0f},  {1.0f, 1.0f}, // bottom left
      {1.0f, 1.0f},   {0.0f, 1.0f}  // top left
  };

  const uint32_t indices[] = {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };

  base.mesh = load_mesh(vertices, 8 * sizeof(*vertices), indices, 6 * sizeof(*indices));

  // Load Default Texture
  unsigned char data[] = {255};
  base.texture = load_texture(data, {1}, 1);

  checkErrors();
}

Renderer::~Renderer() {
  // Delete Textures
  for (auto [id, _] : textures)
    unload_texture(id);

  // Delete Buffers
  for (auto [id, _] : meshes)
    unload_mesh(id);

  // Delete Shaders
  for (const auto &[id, _] : shaders)
    unload_shader(id);
}

void Renderer::clear(vec<3> color) {
  // Clear Screen
  glClearColor(color->red, color->green, color->blue, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

bool Renderer::update() {
  // TODO update rendering stuff
  return window.update();
}

void Renderer::checkErrors() {
  GLenum errorCode;
  while ((errorCode = glGetError()) != GL_NO_ERROR) {
    std::string error;
    switch (errorCode) {
    default:
      error = "UNKNOWN";
      break;
    case GL_INVALID_ENUM:
      error = "INVALID_ENUM";
      break;
    case GL_INVALID_VALUE:
      error = "INVALID_VALUE";
      break;
    case GL_INVALID_OPERATION:
      error = "INVALID_OPERATION";
      break;
    case GL_STACK_OVERFLOW:
      error = "STACK_OVERFLOW";
      break;
    case GL_STACK_UNDERFLOW:
      error = "STACK_UNDERFLOW";
      break;
    case GL_OUT_OF_MEMORY:
      error = "OUT_OF_MEMORY";
      break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      error = "INVALID_FRAMEBUFFER_OPERATION";
      break;
    }
    printf("Renderer::checkErrors; OpenGL Error: %i (%s)\n", errorCode, error.c_str());
  }
}

// TODO basic shader
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