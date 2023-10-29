#include <Artifex/core/Load.hpp>

#include <Artifex/core/Log.hpp>

#include <Artifex/Engine.hpp>
#include <cstring>

#include <SDL2/SDL_mixer.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Artifex {

void Load::init(Engine *pEngine) {
  if (initialized)
    return;

  engine = pEngine;

  stbi_set_flip_vertically_on_load(true);

  initialized = true;
}

void Load::deinit() {
  if (!initialized)
    return;

  // Free Audios
  for (auto audio : engine->resource.audio)
    Mix_FreeChunk(audio);
  engine->resource.audio.clear();

  // Free Musics
  for (auto music : engine->resource.music)
    Mix_FreeMusic(music);
  engine->resource.music.clear();

  // Free Fonts
  for (auto font : engine->resource.font)
    glDeleteTextures(1, &font.data.id);
  engine->resource.font.clear();

  // Free Meshes
  for (auto mesh : engine->resource.mesh)
    mesh = 0;
  engine->resource.mesh.clear();

  // Free Textures
  for (auto t : engine->resource.texture)
    glDeleteTextures(1, &t.id);
  engine->resource.texture.clear();

  // Free Shaders
  for (auto s : engine->resource.shader)
    glDeleteShader(s.id);
  engine->resource.shader.clear();

  initialized = false;
}

uint16_t Load::shader(const char *vertex, const char *fragment,
                      const char *geometry) {
  // Exit if no Shader Code
  const size_t minSize = 8;
  if (strlen(vertex) < minSize || strlen(fragment) < minSize) {
    Log::error("Load::shader", "No Shader Resources");
    return 0;
  }

  // Get Raw Codes
  const char *vert_s = vertex;
  const char *frag_s = fragment;
  const char *geo_s = NULL;

  if (strlen(geometry) >= minSize)
    geo_s = geometry;

  GLuint vert, frag, geo, id;
  bool isGeo = geo_s != NULL;

  int success;
  char infoLog[1024];

  // ---- Compile Vertex Shader
  vert = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert, 1, &vert_s, NULL);
  glCompileShader(vert);

  glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vert, 1024, NULL, infoLog);
    Log::error("Load::shader",
               "Failed to Compile Vertex Shader:\n%s", infoLog);
    return 0;
  }

  // ---- Compile Fragment Shader
  frag = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag, 1, &frag_s, NULL);
  glCompileShader(frag);

  glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(frag, 1024, NULL, infoLog);
    glDeleteShader(vert);
    Log::error("Load::shader",
               "Failed to Compile Fragment Shader:\n%s",
               infoLog);
    return 0;
  }

  // ---- Compile Geometry Shader (if present)
  if (isGeo) {
    geo = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(geo, 1, &geo_s, NULL);
    glCompileShader(geo);

    glGetShaderiv(geo, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(geo, 1024, NULL, infoLog);
      Log::error("Load::shader",
                 "Failed to Compile Geometry Shader:\n%s",
                 infoLog);
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
    glGetProgramInfoLog(id, 1024, NULL, infoLog);
    Log::error("Load::shader", "Failed to Link Shaders:\n%s",
               infoLog);
    return 0;
  }

  Log::verbose("Load::shader", "Loaded Shader", infoLog);

  // Add to list + return ID
  engine->resource.shader.push_back(Shader(id));
  return engine->resource.shader.size() - 1;
}

uint16_t Load::shader(
    const char *path,
    std::unordered_map<std::string, std::string> scripts) {
  FILE *f = fopen(path, "r");

  if (f) {
    uint8_t current = 0;

    // RAW Shader Code (trash, vertex, fragment, geometry)
    std::string code[4];

    char line[256];
    while (fgets(line, sizeof(line), f)) {
      char index[10], parameter[10];
      sscanf(line, "%9s %9s", index, parameter);

      if (!strcmp(index, "#shader")) {
        if (!strcmp(parameter, "vertex"))
          current = 1;
        else if (!strcmp(parameter, "fragment"))
          current = 2;
        else if (!strcmp(parameter, "geometry"))
          current = 3;
        else
          Log::warning("Load::load::shader",
                       "Invalid Parameter: %s", parameter);
      } else if (!strcmp(index, "#script")) {
        if (scripts.count(parameter) != 0)
          code[current] += scripts[parameter];
        else
          Log::warning("Load::load::shader",
                       "Shader Script not given: %s",
                       parameter);
      } else {
        code[current] += line;
      }
    }

    fclose(f);

    return shader(code[1].c_str(), code[2].c_str(),
                  code[3].c_str());
  }

  Log::error("Load::shader", "Failed to Open File");

  return 0;
}

uint16_t Load::texture(unsigned char *data, int width,
                       int height, int channels) {
  // Exit if invalid
  if (data == NULL || (width == 0 || height == 0)
      || (channels < 1 || channels > 4)) {
    Log::error("Load::texture", "Invalid Texture");
    return 0;
  }

  GLuint id;

  // Generate Empty Texture
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);

  // Select Color Mode
  GLuint mode = GL_RGB;
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
  glTexImage2D(GL_TEXTURE_2D, 0, mode, width, height, 0, mode,
               GL_UNSIGNED_BYTE, data);

  // Generate MipMap & Set Parameters
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                  GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  Log::verbose("Load::texture", "Loaded Texture");

  // Add to list + return ID
  engine->resource.texture.push_back(
      (Texture){width, height, channels, id});
  return engine->resource.texture.size() - 1;
}

uint16_t Load::texture(const char *path) {
  int x, y, ch;
  unsigned char *data = stbi_load(path, &x, &y, &ch, 0);

  if (!data)
    return 0;

  return texture(data, x, y, ch);
}

uint16_t Load::music(const char *path) {
  Mix_Music *music = Mix_LoadMUS(path);

  if (!music) {
    Log::error("Load::music", "Failed to open file");
    return 0;
  }

  // Add to list + return ID
  engine->resource.music.push_back(music);
  return engine->resource.music.size() - 1;
}

uint16_t Load::audio(const char *path) {
  // Fix it, there's a bug inside
  Mix_Chunk *chunk = Mix_LoadWAV(path);

  if (!chunk) {
    Log::error("Load::audio", "Failed to open file");
    return 0;
  }

  // Add to list + return ID
  engine->resource.audio.push_back(chunk);
  return engine->resource.audio.size() - 1;
}

// TODO: raw mesh loading

uint16_t Load::mesh(const char *path) {
  // Add to list + return ID
  engine->resource.mesh.push_back(0);
  return engine->resource.mesh.size() - 1;
}

// TODO: raw font loading

uint16_t Load::font(const char *path) {
  // Font out;

  // BMP bmp;
  // int err = bmp_load(&bmp, path, 1);

  // if (err) {
  //     Log::error("Load::font", "Loading font failed with code
  //     %i", err); return 10 + err;
  // }

  // if (bmp.channels != 1) {
  //     Log::error("Load::font", "Failed to load BMP with 1
  //     channel"); return 20;
  // }

  // // Convert to Font
  // out.cols = cols;
  // out.rows = rows;
  // out.data.width = bmp.width;
  // out.data.height = bmp.height;
  // out.data.channels = 1;

  // // Generate OpenGL Texture
  // glGenTextures(1, &out.data.id);
  // glBindTexture(GL_TEXTURE_2D, out.data.id);

  // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, out.data.width,
  // out.data.height, 0,
  //              GL_BGR, GL_UNSIGNED_BYTE, bmp.data);

  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
  // GL_NEAREST); glTexParameteri(GL_TEXTURE_2D,
  // GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  // // Free BMP
  // free(bmp.data);

  // // Load Vertex Data
  // glGenVertexArrays(1, &out.VAO);
  // glGenBuffers(1, &out.VBO);

  // glBindVertexArray(out.VAO);

  // glBindBuffer(GL_ARRAY_BUFFER, out.VBO);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(out.vertices),
  // out.vertices,
  //              GL_STATIC_DRAW);

  // // position attribute
  // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 *
  // sizeof(float),
  //                       (void *)0);
  // glEnableVertexAttribArray(0);
  // // texture coord attribute
  // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 *
  // sizeof(float),
  //                       (void *)(2 * sizeof(float)));
  // glEnableVertexAttribArray(1);

  // Log::system("Load::font", "Loaded Font");

  // // Add to list + return ID
  // engine->resource.font.push_back(out);
  // return engine->resource.font.size() - 1;
  return 0;
}

bool isOneOf(const char *ext,
             std::vector<const char *> supported) {
  for (auto c : supported)
    if (!strcmp(ext, c))
      return true;

  return false;
}

uint16_t Load::load(const char *path, FILE_TYPE type) {
  // Determine File Type
  if (type == FILE_TYPE::AUTO) {
    const char *ext = strrchr(path, '.');
    if (isOneOf(ext, {".glsl", ".shader"}))
      type = FILE_TYPE::SHADER;
#ifdef ARTIFEX_ONLY_BMP
    else if (!strcmp(ext, ".bmp"))
      type = FILE_TYPE::IMAGE;
#else
    else if (isOneOf(ext,
                     {".png", ".jpg", ".jpeg", ".bmp", ".gif"}))
      type = FILE_TYPE::IMAGE;
#endif
    else if (isOneOf(ext, {".mp3", ".ogg", ".wav"}))
      type = FILE_TYPE::AUDIO;
    else if (!strcmp(ext, ".obj"))
      type = FILE_TYPE::MESH;
    else if (!strcmp(ext, ".font"))
      type = FILE_TYPE::FONT;
    else {
      Log::error("Load::load", "Unsupported Type: %s\n", ext);
      return 0;
    }
  }

  uint16_t id = 0;

  switch (type) {
  default:
    Log::warning("Load::load", "Failed to identify file type");
    id = 0;
    break;

  case FILE_TYPE::SHADER:
    id = shader(path);
    break;

  case FILE_TYPE::IMAGE:
    id = texture(path);
    break;

  case FILE_TYPE::AUDIO:
    id = audio(path);
    break;

  case FILE_TYPE::MESH:
    id = mesh(path);
    break;

  case FILE_TYPE::FONT:
    id = font(path);
    break;
  }

  return id;
}

} // namespace Artifex