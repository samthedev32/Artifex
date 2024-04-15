#pragma once

#include <Artifex/graphics/shader.hpp>
#include <Artifex/utility/uuid.h>

#include <cstdint>
#include <unordered_map>

namespace Artifex {

// Basic 2D OpenGL Renderer
class Renderer {
public:
  explicit Renderer(const vec<2, uint32_t> &size);
  ~Renderer();

  // Clear Screen
  void clear(vec<3> color = {});
  bool update();

  // Load RAW Resources
  uuid_t load_shader(const char *vertex, const char *fragment, const char *geometry = nullptr);
  uuid_t load_texture(const void *data, const vec<2, uint32_t> &size, uint8_t channels);
  uuid_t load_mesh(const vec<2, float> *vertices, int vsize, const uint32_t *indices, int isize);

  // Load Resource from File
  uuid_t load_shader(const char *path);

  // Update Resource Data
  //  void update_texture(uuid_t id);
  //  void update_mesh(uuid_t id);

  // Unload Resources
  void unload_shader(uuid_t id);
  void unload_texture(uuid_t id);
  void unload_mesh(uuid_t id);

  // Custom Draw Call
  Shader &select(uuid_t shader); // select (& get) shader
  void draw(uuid_t mesh);        // issue draw call

  // Primitive Shape Appearance Look
  enum Look { DYNAMIC, COLOR, TEXTURE };

  // Draw Default Shape
  void draw(const vec<2> &center, const vec<2> &size, float rotation, Look look, const vec<4> &corner, const vec<3> &color = {},
            uuid_t tex = 0);
  // void draw_mesh(uint32_t id);
  // void draw_image(uint32_t id);
  // void draw_color(const vec<3> &color);

  //  void text(const vec<2> &center, float width, const vec<3> &color, std::string text);

private:
  friend class Engine;

  const vec<2, uint32_t> &framebufferSize;

  // Default Resources
  struct {
    uuid_t shader, mesh, texture;
    uuid_t fontShader;
  } base{};

  struct Mesh {
    int size{};
    unsigned int VAO{}, VBO{}, EBO{};
  };

  std::unordered_map<uuid_t, Shader> shaders{0};
  std::unordered_map<uuid_t, Mesh> meshes{0};
  std::unordered_map<uuid_t, unsigned int> textures{0};

  static void checkErrors();
};

} // namespace Artifex