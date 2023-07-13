// The Resource-Loading Library for Artifex
#pragma once

// #include <Artifex/core/utility.hpp>

#include <cstdint>

class Artifex;

// OpenGL Resource Loader
class Load {
  public:
    Load(Artifex *ax);
    ~Load();

    // Initialize Loader
    void init();

    // Load Resource
    uint16_t load(const char *path);

    // Load Shader
    uint16_t shader(const char *vertex, const char *fragment,
                    const char *geometry = "");

    // Load Texture
    uint16_t texture(unsigned char *data, int width, int height, int channels);

  private:
    Artifex *ax = nullptr;
};