#pragma once

#include <Artifex/core/Window.h>
// #include <Artifex/core/mixer.h>

// Artifex - A Light-Weight, Cross-Platform 2D Game Engine
class Artifex : public Window //, public Mixer
{
private:
  long past, now; //< The Previous Update's Time

  uint VAO, VBO; //< The Rendering Vertex-Array-Object and Vertex-Buffer-Object
  // font tfont;

  std::map<std::string, int>
      input; //< Temporary Variables for Buttons and Interactables

  // Apply 2D Renderer
  void apply();

  // Get Screen Ratio Multiplyer
  float ratio();

public:
  long uptime;
  float deltaTime;

  shader s;

  bool dres = true;

  // Constructors & Destructor

  Artifex(std::string name, bool fullscreen = true, int w = 0, int h = 0);
  Artifex(std::string name, int w, int h);
  ~Artifex();

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