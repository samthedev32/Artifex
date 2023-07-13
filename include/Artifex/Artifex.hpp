#pragma once

#include <Artifex/core/Window.hpp>

#include <Artifex/core/Load.hpp>
#include <Artifex/core/Render.hpp>

#include <cstdint>
#include <unordered_map>
#include <vector>

// 2D Game Engine
class Artifex : public Window {
  public:
    Artifex(std::string name, int width = 0, int height = 0);
    ~Artifex();

    bool update(float r = 0.0f, float g = 0.0f, float b = 0.0f);

    float time();
    float ratio();

    Load load;
    Render render;

  public:
    float deltaTime;

    std::vector<Shader> shader;
    std::vector<GLuint> texture;

  private:
    float past, now;

    std::unordered_map<std::string, int> input;
};