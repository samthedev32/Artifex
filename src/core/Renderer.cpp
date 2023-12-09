#include <Artifex/core/Renderer.hpp>

#include <Artifex/Engine.hpp>

#include <cstring>
#include <ctime>

// Radian-Degree Conversion Functions
#define rads(d) (d * M_PI / 180.0f)
#define degs(r) (r * 180.0f / M_PI)

namespace Artifex {

Renderer::Renderer(const std::string &name, const vec<2, uint32_t> &size) : Window(name, size) {
  // Load Default Rect
  vec<2> vertices[] = {
      // positions      // texture coords
      {-1.0f, 1.0f},  {0.0f, 1.0f}, // top right
      {-1.0f, -1.0f}, {0.0f, 0.0f}, // bottom right
      {1.0f, -1.0f},  {1.0f, 0.0f}, // bottom left
      {1.0f, 1.0f},   {1.0f, 1.0f}  // top left
  };

  int indices[] = {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
  // texture coord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Config OpenGL
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_CULL_FACE);

  // Load 2D Shader
  // TODO: default shader
  // engine->load.shader(default_shader_code[0],
  // default_shader_code[1]);
  //  engine->load.shader("../../../examples/hello_world/shader/2d.glsl");

  // Load Default Texture
  unsigned char data[] = {255};
  texture(data, {1}, 1);
}

Renderer::~Renderer() {
  // Delete Buffers
  GLuint buffers[] = {VAO, VBO};
  glDeleteBuffers(2, buffers);
}

void Renderer::clear(vec<3> color) {
  // Clear Screen
  glClearColor(color->red, color->green, color->blue, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

size_t Renderer::shader(const char *vertex, const char *fragment, const char *geometry) {
  // Exit if no Shader Code
  const size_t minSize = 8;
  if (strlen(vertex) < minSize || strlen(fragment) < minSize) {
    Log::error("Load::shader", "No Shader Resources");
    return 0;
  }

  bool isGeo = strlen(geometry) >= minSize;

  GLuint vert, frag, geo, id;

  int success;
  char infoLog[1024];

  // ---- Compile Vertex Shader
  vert = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert, 1, &vertex, nullptr);
  glCompileShader(vert);

  glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vert, 1024, nullptr, infoLog);
    Log::error("Load::shader", "Failed to Compile Vertex Shader:\n%s", infoLog);
    return 0;
  }

  // ---- Compile Fragment Shader
  frag = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag, 1, &fragment, nullptr);
  glCompileShader(frag);

  glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(frag, 1024, nullptr, infoLog);
    glDeleteShader(vert);
    Log::error("Load::shader", "Failed to Compile Fragment Shader:\n%s", infoLog);
    return 0;
  }

  // ---- Compile Geometry Shader (if present)
  if (isGeo) {
    geo = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(geo, 1, &geometry, nullptr);
    glCompileShader(geo);

    glGetShaderiv(geo, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(geo, 1024, nullptr, infoLog);
      Log::error("Load::shader", "Failed to Compile Geometry Shader:\n%s", infoLog);
      isGeo = false;
    }
  }

  // ---- Create Shader Program
  id = glCreateProgram();
  glAttachShader(id, vert);
  glAttachShader(id, frag);
  if (isGeo)
    glAttachShader(id, geo);

  glLinkProgram(id);

  // Delete unnecessary shaders
  glDeleteShader(vert);
  glDeleteShader(frag);
  if (isGeo)
    glDeleteShader(geo);

  // Check if linking was successful
  glGetProgramiv(id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(id, 1024, nullptr, infoLog);
    Log::error("Load::shader", "Failed to Link Shaders:\n%s", infoLog);
    return 0;
  }

  Log::verbose("Load::shader", "Loaded Shader", infoLog);

  // Add to list + return ID
  shaders.emplace_back(id);
  return shaders.size() - 1;
}

size_t Renderer::texture(void *data, const vec<2, uint32_t> &size, uint8_t channels) {
  // Exit if invalid
  if (data == nullptr || (size->width == 0 || size->height == 0) || (channels < 1 || channels > 4)) {
    Log::error("Load::texture", "Invalid Texture");
    return 0;
  }

  GLuint id;

  // Generate Empty Texture
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);

  // Select Color Mode
  GLint mode = GL_RGB;
  switch (channels) {
  case 1:
    mode = GL_RED;
    break;

  case 2:
    mode = GL_RG;
    break;

  default:
  case 3:
    mode = GL_RGB;
    break;

  case 4:
    mode = GL_RGBA;
    break;
  }

  // Create Texture
  glTexImage2D(GL_TEXTURE_2D, 0, mode, (int)size->width, (int)size->height, 0, mode, GL_UNSIGNED_BYTE, data);

  // Generate MipMap & Set Parameters
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  Log::verbose("Load::texture", "Loaded Texture");

  // Add to list + return ID
  textures.push_back(id);
  return textures.size() - 1;
}

void Renderer::roundable(const vec<2> &center, const vec<2> &size, int look, const vec<3> &color, uint16_t tex, float amount,
                         float rotation) {
  shaders[0].use();

  // Vertex
  shaders[0].set("v.center", center);
  shaders[0].set("v.size", size);
  shaders[0].set("v.ratio", vec<2>(1.0f, (float)Window::size->width / (float)Window::size->height));
  shaders[0].set("v.rotation", (float)rads(rotation));

  // Fragment
  shaders[0].set("f.look", look);
  shaders[0].set("f.corner", amount);
  shaders[0].set("f.time", 0); // TODO time

  switch (look) {
  default:
  case 0:
    break;

  case 1:
    shaders[0].set("f.color", color);
    break;

  case 2:
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[tex]);
    shaders[0].set("f.tex", textures[tex]);
    break;
  }

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, nullptr);
}

// void Renderer::text(const vec<2> &center, float width, const vec<3> &color, float rotation) {
//   engine->resource.shader[engine->current.shader].use();
//
//   glActiveTexture(GL_TEXTURE0);
//   glBindTexture(GL_TEXTURE_2D, engine->resource.font[0].data.id);
//
//   engine->resource.shader[engine->current.shader].set("v.type", 2);
//   engine->resource.shader[engine->current.shader].set("color", color);
//   engine->resource.shader[engine->current.shader].set("ratio", engine->ratio());
//
//   // WARNING: Possible Div by 0
//   // float step = width / text.size();
//   // float left = center.x - step * text.size() / 2.0f;
//
//   // // iterate through all characters
//   // for (int i = 0; i < (int)text.size(); i++) {
//   //     char c = text[i] - tfont.start;
//
//   //     // Set Position
//   //     s.set("center", vec<2>(left + i * step, center.y));
//   //     s.set("size", vec<2>(step, step * height));
//
//   //     // WARNING: Possible Divs by 0
//   //     vec<2> start = {(c % tfont.width) / (float)tfont.width,
//   //                   (c / tfont.height + 1) /
//   //                   (float)tfont.height};
//   //     vec<2> csize = {1.0f / (float)tfont.width, 1.0f /
//   //     (float)tfont.height};
//
//   //     tfont.vertices[2] = start.x;
//   //     tfont.vertices[3] = 1 - start.y + csize.y;
//   //     tfont.vertices[6] = start.x;
//   //     tfont.vertices[7] = 1 - start.y;
//   //     tfont.vertices[10] = start.x + csize.x;
//   //     tfont.vertices[11] = 1 - start.y;
//
//   //     tfont.vertices[14] = start.x + csize.x;
//   //     tfont.vertices[15] = 1 - start.y + csize.y;
//
//   //     glBindBuffer(GL_ARRAY_BUFFER, tfont.VBO);
//   //     glBufferData(GL_ARRAY_BUFFER, sizeof(tfont.vertices),
//   //     tfont.vertices,
//   //                  GL_STATIC_DRAW);
//
//   //     glBindVertexArray(tfont.VAO);
//   //     // glDrawArrays(GL_TRIANGLES, 0, 6);
//   //     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//   // }
// }

} // namespace Artifex