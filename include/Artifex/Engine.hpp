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

    bool update(vec3 clearColor);

    float time();
    float ratio();

    Load load;
    Render render;
    UI ui;

  public:
    float deltaTime;

    std::vector<Shader> shader;
    std::vector<GLuint> texture;

  private:
    float past, now;
};

} // namespace Artifex