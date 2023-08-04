// Windowing Library for Artifex
#pragma once

// Detect OS
#include <unordered_map>
#if defined(__linux)
#define __pc
#elif defined(__WIN32)
#define __windows
#define __pc
#elif defined(__EMSCRIPTEN__)
#define __webassembly
#define __browser
#include <emscripten.h>
#include <emscripten/html5.h>
#else
#warning Targeted OS is not recognized (may not work)
#endif

#include <GL/glad.h>
#include <mathutil/common.hpp>

#include <GLFW/glfw3.h>
#include <map>
#include <set>
#include <string>

// TODO: make mouse use "vec2"

namespace Artifex {

class Window {
  public:
    // Create Window
    Window(std::string name, int width = 0, int height = 0);

    // Destroy Window
    ~Window();

    // Update Window
    bool update();

    // Close Window
    void exit(bool sure = true);

    // Enable/Disable Fullscreen Mode
    void fullscreen(bool en = true);

    // Enable/Disable VSync
    void vsync(int interval = 1);

    // Get Key State
    bool key(std::string k);

  public:
    // Current Width & Height of Window
    int width, height;

    // Cursor & Scroll Positions
    vec2 cursor, scroll;

  private:
    GLFWwindow *window = nullptr;
    int small_size[2];

    std::set<int> keyboard;

    bool mouseState[3];

  private:
    std::unordered_map<std::string, int> GLFW_STRING_SCANCODE;

    static void callback_resize(GLFWwindow *window, int w, int h);
    static void callback_key(GLFWwindow *window, int key, int scancode,
                             int action, int mods);
    static void callback_cursor(GLFWwindow *window, double x, double y);
    static void callback_scroll(GLFWwindow *window, double x, double y);
};

} // namespace Artifex