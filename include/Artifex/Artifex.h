#pragma once

#include <Artifex/core/Window.h>
#include <Artifex/types/shader.hpp>

#include <mathutil/vector.hpp>

#include <stdint.h>
#include <unordered_map>
#include <vector>

// #include <Artifex/core/mixer.h>

#define AX_TYPE_AUTO 0
#define AX_TYPE_SHADER 1
#define AX_TYPE_TEXTURE 2

// Artifex - A Light-Weight, Cross-Platform 2D Game Engine
class Artifex : public Window {
  public:
    Artifex(std::string name, uint width = 0, uint height = 0);
    ~Artifex();

    bool update(float r = 0.0f, float g = 0.0f, float b = 0.0f);

    float time();

    // ---- Resource Loading

    // Load Resource
    uint16_t load(const char *path, uint8_t type = AX_TYPE_AUTO);

    // Load Shader
    uint16_t load_shader(const char *vertex, const char *fragment,
                         const char *geometry = "");

    // Load Texture
    uint16_t load_texture(unsigned char *data, int width, int height,
                          int channels);

    // ---- Rendering

    void rect(vec2 center, vec2 size, vec3 color, float rotation = 0.0f);
    void rect(vec2 center, vec2 size, uint16_t tex, float rotation = 0.0f);

  public:
    float deltaTime;

  private:
    float past, now;

    GLuint VAO, VBO, EBO;

    std::map<std::string, int> input;

    std::vector<Shader> shader;

    std::vector<GLuint> texture;

  private:
    float ratio();
};