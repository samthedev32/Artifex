#pragma once

#include <Artifex/core/Window.h>
// #include <Artifex/core/mixer.h>

// Artifex - A Light-Weight, Cross-Platform 2D Game Engine
class Artifex : public Window {
  public:
    Artifex(std::string name, uint width = 0, uint height = 0);
    ~Artifex();

    bool update();

    void clear(float red = 0.0f, float green = 0.0f, float blue = 0.0f,
               GLbitfield buffers = GL_COLOR_BUFFER_BIT);

    // TODO: Resource Managament, Rendering

  public:
    long uptime;
    float deltaTime;

  private:
    long past, now;

    uint VAO, VBO;

    std::map<std::string, int> input;

    // Apply 2D Renderer
    void apply();

    // Get Screen Ratio Multiplyer
    float ratio();
    //   bool update(vec3 color = vec3());

    //   // Basic 2D Rendering
    //   void circle(vec2 center, float radius, float cutradius,
    //               vec3 color); //< Draw Circle
    //   void rect(vec2 center, vec2 size, vec3 color,
    //             float scale = 1.0f); //< Draw Colored Rectangle
    //   void rect(vec2 center, vec2 size, texture tex,
    //             float scale = 1.0f); //< Draw Textured Rectangle
    //   void rect(vec2 center, vec2 size, texture *texs, uint n, uint speed,
    //             float scale = 1.0f); //< Draw Animated Rectangle
    //   void text(vec2 center, float size, std::string text, vec3 color,
    //             float height = 1.5f);

    //   // User Inputs
    //   bool button(vec2 center, vec2 size, std::string name,
    //               texture a = load::rgb({0, 1}), texture b = load::rgb(1));
    //   bool toggle(vec2 center, vec2 size, std::string name,
    //               texture a = load::rgb({0, 1}), texture b = load::rgb(1));
    //   bool touch(vec2 center, vec2 size, texture a = load::rgb({0, 1}),
    //              texture b = load::rgb(1));
    //   vec2 joystick(vec2 center = vec2(), std::string name = "joystick",
    //                 float radius = 0.2f, float nob_radius = 0.04f,
    //                 vec3 color = vec3(1.0f, 0.4f, 0.0f),
    //                 vec3 nob_color = vec3(1.0f, 1.0f, 1.0f));
};