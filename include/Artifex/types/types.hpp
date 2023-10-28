#pragma once

#include <map>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include <Artifex/types/shader.hpp>

namespace Artifex {

class Engine;

// OpenGL Texture ID
struct Texture {
  int width, height, channels;

  GLuint id;
};

// Text Font
struct Font {
  Texture data;
  uint8_t rows, cols;

  vec2 VERT[6];
  vec2 UV[6];

  GLuint VAO, VBO;

  const struct {
    vec2 pos, uv;
  } vertices[6] = {{{-1.0f, 1.0f}, {0.0f, 1.0f}},
                   {{-1.0f, -1.0f}, {0.0f, 0.0f}},
                   {{1.0f, -1.0f}, {1.0f, 0.0f}},

                   {{-1.0f, 1.0f}, {0.0f, 1.0f}},
                   {{1.0f, -1.0f}, {1.0f, 0.0f}},
                   {{1.0f, 1.0f}, {1.0f, 1.0f}}};
};

// Module
class Module {
public:
  Engine *ax;

public:
  virtual bool onCreate() { return true; }

  virtual void onDestroy() {}

  virtual bool onUpdate(float deltaTime) { return true; }
};

} // namespace Artifex