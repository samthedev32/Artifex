//
// Created by SamTheDev on 11/12/2023.
//

#include <Artifex/core/Renderer.hpp>

#define GLAD_IMPLEMENTATION
#include "GL/glad.h"

#define STB_IMAGE_IMPLEMENTATION
#include "GL/stb_image.h"

#include <cstring>

namespace Artifex {

uuid_t Renderer::load_shader(const char *vertex, const char *fragment, const char *geometry) {
  // Exit if no Shader Code
  constexpr size_t minSize = 8;
  if (strlen(vertex) < minSize || strlen(fragment) < minSize) {
    Log::error("Load::shader", "No Shader Resources");
    return 0;
  }

  bool isGeo = geometry && strlen(geometry) >= minSize;

  GLuint geo{};

  int success;
  char infoLog[1024];

  // ---- Compile Vertex Shader
  const GLuint vert = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert, 1, &vertex, nullptr);
  glCompileShader(vert);

  glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vert, 1024, nullptr, infoLog);
    Log::error("Load::shader", "Failed to Compile Vertex Shader:\n%s", infoLog);
    return 0;
  }

  // ---- Compile Fragment Shader
  const GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
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
  GLuint id = glCreateProgram();
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
  uuid_t uuid = uuid_generate();
  shaders[uuid] = Shader(id);
  return uuid;
}

uuid_t Renderer::load_shader(const char *path) {
  if (FILE *f = fopen(path, "r")) {
    int current = -1;

    // RAW Shader Code (vertex, fragment, geometry)
    std::string code[3];

    char line[256];
    while (fgets(line, sizeof(line), f)) {
      char index[10], parameter[10];
      sscanf(line, "%9s %9s", index, parameter);

      if (!strcmp(index, "#shader")) {
        if (!strcmp(parameter, "vertex"))
          current = 0;
        else if (!strcmp(parameter, "fragment"))
          current = 1;
        else if (!strcmp(parameter, "geometry"))
          current = 2;
        // else
        // Log::warning("Main/Load", "Invalid Shader type: %s", parameter);
        // } else if (!strcmp(index, "#script")) {
        // Log::warning("Main/Load", "Shader Scripts are not supported YET!\n");
      } else {
        if (-1 != current)
          code[current] += line;
      }
    }

    fclose(f);

    return load_shader(code[0].c_str(), code[1].c_str(), code[2].c_str());
  }

  // Failed to open file
  return base.shader;
}

size_t Renderer::load_texture(void *data, const vec<2, uint32_t> &size, uint8_t channels) {
  // Exit if invalid
  // TODO implement vec comparations
  if (data == nullptr || (size->width == 0 || size->height == 0) || (channels < 1 || channels > 4)) {
    Log::error("Load::texture", "Invalid Texture");
    return 0;
  }

  GLuint id;

  // Generate Empty Texture
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);

  // Select Color Mode
  GLint mode;
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
  uuid_t uuid = uuid_generate();
  textures[uuid] = id;
  return uuid;
}

uuid_t Renderer::load_texture(const char *path) {
  vec<2, int> size;
  int ch;
  stbi_set_flip_vertically_on_load(true);
  void *image = stbi_load(path, &size->width, &size->height, &ch, 3);
  return load_texture(image, size, ch);
}

size_t Renderer::load_mesh(vec<2, float> *vertices, int vsize, uint32_t *indices, int isize) {
  Renderer::Mesh o;
  o.size = isize;

  glGenVertexArrays(1, &o.VAO);
  glGenBuffers(1, &o.VBO);
  glGenBuffers(1, &o.EBO);

  glBindVertexArray(o.VAO);

  glBindBuffer(GL_ARRAY_BUFFER, o.VBO);
  glBufferData(GL_ARRAY_BUFFER, vsize, vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, o.EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, isize, indices, GL_STATIC_DRAW);

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

  uuid_t uuid = uuid_generate();
  meshes[uuid] = o;
  return uuid;
}

} // namespace Artifex