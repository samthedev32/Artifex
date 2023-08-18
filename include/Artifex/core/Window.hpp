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
#warning Targeted OS is not recognized
#endif

#include <mathutil/common.hpp>

#include <GL/glad.h>
#include <GLFW/glfw3.h>

#include <map>
#include <set>
#include <string>

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

    // TODO: make emscripten compatible
    // https://github.com/emscripten-core/emscripten/blob/main/test/third_party/glbook/Common/esUtil.h

  private:
    GLFWwindow *window = nullptr;
    int small_size[2];

    std::set<int> keyboard;

    bool mouseState[3];

  private:
    static void callback_resize(GLFWwindow *window, int w, int h);
    static void callback_key(GLFWwindow *window, int key, int scancode,
                             int action, int mods);
    static void callback_cursor(GLFWwindow *window, double x, double y);
    static void callback_scroll(GLFWwindow *window, double x, double y);
};

} // namespace Artifex