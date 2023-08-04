#pragma once

#include <mathutil/common.hpp>

#include <string>
#include <unordered_map>

namespace Artifex {

class Engine;

struct UI {
  public:
    // Initialize UI
    void init(Engine *artifex);

    // DeInitialize UI
    void deinit();

    // Push Button
    bool button(vec2 center, vec2 size, std::string name, uint16_t a,
                uint16_t b);

    // Toggle Switch
    bool toggle(vec2 center, vec2 size, std::string name, uint16_t a,
                uint16_t b);

    // Touch
    bool touch(vec2 center, vec2 size, uint16_t a, uint16_t b);

    // Joystick
    // vec2 joystick(vec2 center = vec2(), std::string name = "joystick",
    //               float radius = 0.2f, float nob_radius = 0.04f,
    //               vec3 color = vec3(1.0f, 0.4f, 0.0f),
    //               vec3 nob_color = vec3(1.0f, 1.0f, 1.0f));

  private:
    bool initialized = false;
    Engine *ax;

    std::unordered_map<std::string, int> input;
};

} // namespace Artifex