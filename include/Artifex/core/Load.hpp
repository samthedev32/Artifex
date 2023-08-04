#pragma once

#include <cstdint>

namespace Artifex {

class Engine;

struct Load {
  public:
    // Initialize Loader
    void init(Engine *pEngine);

    // DeInitialize Loader
    void deinit();

    // Load Resource
    uint16_t load(const char *path);

    // Load Shader
    uint16_t shader(const char *vertex, const char *fragment,
                    const char *geometry = "");

    // Load Texture
    uint16_t texture(unsigned char *data, int width, int height, int channels);

  private:
    bool initialized = false;
    Engine *engine = nullptr;
};

} // namespace Artifex