#pragma once

#include <Artifex/core/Window.h>

#include <vector>
// #include <Artifex/core/mixer.h>

// Artifex - A Light-Weight, Cross-Platform 2D Game Engine
class Artifex : public Window {
  public:
    Artifex(std::string name, uint width = 0, uint height = 0);
    ~Artifex();

    bool update();

    void clear(float red = 0.0f, float green = 0.0f, float blue = 0.0f,
               GLbitfield buffers = GL_COLOR_BUFFER_BIT);

    // Resource Loading
    uint shader(std::string vertex, std::string fragment,
                std::string geometry = "");

    uint texture(uint *data, uint width, uint height, uint nrChannels);

    // TODO: Resource Managament, Rendering

  public:
    long uptime;
    float deltaTime;

  private:
    long past, now;

    uint VAO, VBO;

    std::map<std::string, int> input;

    std::vector<uint> shaders;
    std::vector<uint> textures;

  private:
    float ratio();
};