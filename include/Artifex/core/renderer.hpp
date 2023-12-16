#pragma once

#include <Artifex/core/window.hpp>
#include <Artifex/types/shader.hpp>

#include <cstdint>
#include <vector>

namespace Artifex {

// OpenGL Renderer
class Renderer : public Window {
public:
  Renderer(const std::string &name, const vec<2, uint32_t> &size);
  ~Renderer();

  // Clear Screen
  static void clear(vec<3> color);

  // Load Resource
  struct load {
    explicit load(Renderer &renderer) : renderer(renderer) {}

    size_t shader(const char *vertex, const char *fragment, const char *geometry = nullptr);
    size_t texture(void *data, const vec<2, uint32_t> &size, uint8_t channels);
    size_t mesh(vec<2, float> *vertices, int vsize, uint32_t *indices, int isize);

  private:
    Renderer &renderer;
  } load;

  // Primitive Shapes
  enum Shape {
    RECT = 0, // Simple Rectangle
    CIRCLE,   // Simple Circle
    TRIANGLE, // Simple Triangle
    CARD,     // Rectangle with Rounded Corners
  };

  // Render Something
  struct draw {
    explicit draw(Renderer &renderer) : renderer(renderer) {}

    //    void mesh(uint32_t id);
    //    void image(uint32_t id);
    void color(const vec<3> &color);

    //    void text(const vec<2> &center, float width);

  private:
    Renderer &renderer;
  } draw;

private:
  struct meshData {
    GLsizei size{};
    GLuint VAO{}, VBO{}, EBO{};
  };

  std::vector<Shader> shaders;
  std::vector<GLuint> textures;
  std::vector<meshData> meshes;
};

} // namespace Artifex