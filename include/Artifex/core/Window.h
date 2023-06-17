// Windowing Library for Artifex
#pragma once

// Detect OS
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

#include <GLFW/glfw3.h>
#include <map>
#include <set>
#include <string>

// OpenGL Windowing Class
class Window {
  public:
    Window(std::string name, uint width = 0, uint height = 0);
    ~Window();

    bool update(float r = 0.0f, float g = 0.0f, float b = 0.0f);
    void exit(bool sure = true);

    void fullscreen(bool en = true);
    void vsync(int interval = 1);

    bool key(std::string k);

  public:
    int width, height;

    double cursor[2], scroll[2];

  private:
    GLFWwindow *window = nullptr;
    int small_size[2];

    std::set<int> keyboard;

    bool mouseState[3];

  private:
    std::map<std::string, int> GLFW_STRING_SCANCODE;

    static void callback_resize(GLFWwindow *window, int w, int h);
    static void callback_key(GLFWwindow *window, int key, int scancode,
                             int action, int mods);
    static void callback_cursor(GLFWwindow *window, double x, double y);
    static void callback_scroll(GLFWwindow *window, double x, double y);
};