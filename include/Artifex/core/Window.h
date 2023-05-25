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
  // Window Width and Height
  int width, height;

  Window(std::string name, bool fullscreen = true, int w = 720, int h = 480);
  ~Window();

  bool update(float r = 0.0f, float g = 0.0f, float b = 0.0f);
  void exit(bool sure = true);

  double cursor[2];
  long scroll[2];

  bool key(std::string k);

private:
  GLFWwindow *window = nullptr;
  GLFWmonitor *monitor = nullptr;

  std::set<int> keyboard;

  bool mouseState[3];

private:
  // FrameBuffer Size Callback
  static void callback_resize(GLFWwindow *window, int w, int h) {
    Window *self = (Window *)glfwGetWindowUserPointer(window);
    self->width = w;
    self->height = h;
  }

  // Keyboard State Callback
  static void callback_key(GLFWwindow *window, int key, int scancode,
                           int action, int mods) {
    Window *self = (Window *)glfwGetWindowUserPointer(window);

    if (action == GLFW_PRESS)
      self->keyboard.insert(scancode);
    else
      self->keyboard.erase(scancode);
  }

  // Cursor Movement Callback
  static void callback_cursor(GLFWwindow *window, double x, double y) {
    Window *self = (Window *)glfwGetWindowUserPointer(window);

    self->mouse.x = (x * 2.0f) / (float)self->width - 1.0f;
    self->mouse.y = (y * -2.0f) / (float)self->height + 1.0f;
  }

  // Scroll Movement Callback
  static void callback_scroll(GLFWwindow *window, double x, double y) {
    Window *self = (Window *)glfwGetWindowUserPointer(window);

    self->scroll.x = x;
    self->scroll.y = y;
  }
};