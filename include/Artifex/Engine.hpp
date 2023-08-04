#pragma once

#include <Artifex/core/Window.hpp>

#include <Artifex/core/Load.hpp>
#include <Artifex/core/Render.hpp>
#include <Artifex/core/UI.hpp>

#include <cstdint>
#include <unordered_map>
#include <vector>

// TODO: physics

namespace Artifex {

class Engine : public Window {
  public:
    Engine(std::string name, int width = 0, int height = 0);
    ~Engine();

    // Update Engine & Window
    bool update(vec3 clearColor);

    // Get Current Time (s)
    float time();

    // Get Ratio of Window (width/height)
    inline float ratio() { return (float)width / (float)height; }

    // Resource Loader
    Load load;

    // Basic Shape Renderer
    Render render;

    // UI Renderer
    UI ui;

  public:
    // Time when frame started
    float now;

    // Duration of Last Frame
    float deltaTime;

    // Loaded Shaders
    std::vector<Shader> shader;

    // Loaded Textures
    std::vector<GLuint> texture;

  private:
    float past;
};

} // namespace Artifex