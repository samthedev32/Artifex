#pragma once

#include <Artifex/types/types.hpp>
#include <cstdint>

namespace Artifex {

class Engine;

enum class FILE_TYPE {
    AUTO,
    SHADER,
    IMAGE,
    FONT,
};

struct Load {
  public:
    // Initialize Loader
    void init(Engine *pEngine);

    // DeInitialize Loader
    void deinit();

    // Load Shader(s)
    uint16_t shader(const char *vertex, const char *fragment,
                    const char *geometry = "");

    // Load Compact Shader from file
    uint16_t shader(const char *path);

    // Load RAW Texture
    uint16_t texture(unsigned char *data, int width, int height, int channels);

    // Load Texture from file
    uint16_t texture(const char *path);

    // TODO: font loading
    uint16_t font(const char *path);

    // Load Resource
    uint16_t load(const char *path, FILE_TYPE type = FILE_TYPE::AUTO);

  private:
    bool initialized = false;
    Engine *engine = nullptr;
};

} // namespace Artifex