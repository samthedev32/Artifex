#pragma once

#include <Artifex/core/Window.hpp>

#include <Artifex/core/Load.hpp>
#include <Artifex/core/Render.hpp>
#include <Artifex/core/UI.hpp>

#include <box2d/box2d.h>

#include <cstdint>
#include <unordered_map>
#include <vector>

// TODO: Integrate Box2D Physics Engine
// TODO: Custom Physics Engine

// 2D Game Engine
class Artifex : public Window {
  public:
    Artifex(std::string name, int width = 0, int height = 0);
    ~Artifex();

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

    std::unordered_map<std::string, int> input;
};