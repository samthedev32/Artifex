#include <Artifex/core/window.hpp>
#include <Artifex/utility/Log.hpp>

#define GLAD_IMPLEMENTATION
#include "GL/glad.h"

#include <unordered_map>

namespace Artifex {

extern std::unordered_map<std::string, int> GLFW_STRING_SCANCODE;

Window::Window(const std::string &name, vec<2, uint32_t> size) : size(size) {
  // Decide Window Size
  size->width = size->width != 0 ? size->width : 1;
  size->height = size->height != 0 ? size->height : 1;

  smallSize = size;

  // Init GLFW
  glfwInit();

  // Create Window
  window = glfwCreateWindow(static_cast<int>(size->width), static_cast<int>(size->height), name.c_str(), nullptr, nullptr);

  // Exit if Window Creation Failed
  if (window == nullptr) {
    Log::error("Window", "Failed to create window: %i", glfwGetError(nullptr));
    glfwTerminate();
    return;
  }

  // Get Actual Window Sizes
  int w, h;
  glfwGetWindowSize(window, &w, &h);
  this->size = {w, h};
  ratio = static_cast<float>(w) / static_cast<float>(h);

  // Make OpenGL Context
  glfwMakeContextCurrent(window);

  // Request Modern OpenGL
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

  // Load OpenGL (exit if failed)
  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    Log::error("Window", "Failed to init OpenGL: %u", glGetError());
    glfwDestroyWindow(window);
    glfwTerminate();
    return;
  }

  // Set Window User Pointer
  glfwSetWindowUserPointer(window, this);

  // Set Callbacks
  glfwSetFramebufferSizeCallback(window, callback_resize);
  glfwSetKeyCallback(window, callback_key);
  glfwSetCursorPosCallback(window, callback_cursor);
  glfwSetScrollCallback(window, callback_scroll);
}

Window::~Window() {
  if (window)
    glfwDestroyWindow(window);
  glfwTerminate();
}

bool Window::update() {
  glfwSwapBuffers(window);

  glfwPollEvents();

  int w, h;
  glfwGetFramebufferSize(window, &w, &h);
  size = {w, h};

  return !glfwWindowShouldClose(window);
}

void Window::exit(const bool sure) const { glfwSetWindowShouldClose(window, sure); }

void Window::fullscreen(const bool en, uint8_t hiddenCursor, int minWidth, int minHeight) {
  if (en) {
    if (!isFullscreen) {
      // Save Window Size
      smallSize = size;

      // Get Monitor
      GLFWmonitor *monitor = glfwGetWindowMonitor(window);
      if (monitor == nullptr)
        monitor = glfwGetPrimaryMonitor();

      const GLFWvidmode *videoMode = glfwGetVideoMode(monitor);

      // Make Fullscreen
      glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, videoMode->width, videoMode->height, GLFW_DONT_CARE);

      isFullscreen = true;
    }
  } else {
    if (isFullscreen) {
      // Undo Fullscreen
      glfwSetWindowMonitor(window, nullptr, 0, 0, static_cast<int>(smallSize->width), static_cast<int>(smallSize->height),
                           GLFW_DONT_CARE);

      isFullscreen = false;
    }
  }
}

void Window::vsync(const int interval) { glfwSwapInterval(interval); }

bool Window::key(const std::string &k) const { return keyboard.count(glfwGetKeyScancode(GLFW_STRING_SCANCODE[k])) > 0; }

// Callbacks

void Window::callback_resize(GLFWwindow *window, int w, int h) {
  auto *self = static_cast<Window *>(glfwGetWindowUserPointer(window));
  self->size = {w, h};
  self->ratio = static_cast<float>(w) / static_cast<float>(h);
  glViewport(0, 0, w, h);
}

void Window::callback_key(GLFWwindow *window, int key, int scancode, int action, int mods) {
  auto *self = static_cast<Window *>(glfwGetWindowUserPointer(window));

  if (action == GLFW_PRESS)
    self->keyboard.insert(scancode);
  else if (action == GLFW_RELEASE)
    self->keyboard.erase(scancode);
}

void Window::callback_cursor(GLFWwindow *window, double x, double y) {
  auto *self = static_cast<Window *>(glfwGetWindowUserPointer(window));

  self->cursor = {(x * 2.0) / (double)self->size->width - 1.0, (y * -2.0) / (double)self->size->height + 1.0};
}

void Window::callback_scroll(GLFWwindow *window, double x, double y) {
  auto *self = (Window *)glfwGetWindowUserPointer(window);

  self->scroll = {x, y};
}

std::unordered_map<std::string, int> GLFW_STRING_SCANCODE = {
    {"a", GLFW_KEY_A},
    {"b", GLFW_KEY_B},
    {"c", GLFW_KEY_C},
    {"d", GLFW_KEY_D},
    {"e", GLFW_KEY_E},
    {"f", GLFW_KEY_F},
    {"g", GLFW_KEY_G},
    {"h", GLFW_KEY_H},
    {"i", GLFW_KEY_I},
    {"j", GLFW_KEY_J},
    {"k", GLFW_KEY_K},
    {"l", GLFW_KEY_L},
    {"m", GLFW_KEY_M},
    {"n", GLFW_KEY_N},
    {"o", GLFW_KEY_O},
    {"p", GLFW_KEY_P},
    {"q", GLFW_KEY_Q},
    {"r", GLFW_KEY_R},
    {"s", GLFW_KEY_S},
    {"t", GLFW_KEY_T},
    {"u", GLFW_KEY_U},
    {"v", GLFW_KEY_V},
    {"w", GLFW_KEY_W},
    {"x", GLFW_KEY_X},
    {"y", GLFW_KEY_Y},
    {"z", GLFW_KEY_Z},

    {"0", GLFW_KEY_0},
    {"1", GLFW_KEY_1},
    {"2", GLFW_KEY_2},
    {"3", GLFW_KEY_3},
    {"4", GLFW_KEY_4},
    {"5", GLFW_KEY_5},
    {"6", GLFW_KEY_6},
    {"7", GLFW_KEY_7},
    {"8", GLFW_KEY_8},
    {"9", GLFW_KEY_9},

    {"return", GLFW_KEY_ENTER},
    {"enter", GLFW_KEY_ENTER},
    {"\n", GLFW_KEY_ENTER},

    {"escape", GLFW_KEY_ESCAPE},
    {"esc", GLFW_KEY_ESCAPE},

    {"backspace", GLFW_KEY_BACKSPACE},
    {"\b", GLFW_KEY_BACKSPACE},

    {"tab", GLFW_KEY_TAB},
    {"\t", GLFW_KEY_TAB},

    {"space", GLFW_KEY_SPACE},
    {" ", GLFW_KEY_SPACE},

    {"up", GLFW_KEY_UP},
    {"down", GLFW_KEY_DOWN},
    {"right", GLFW_KEY_RIGHT},
    {"left", GLFW_KEY_LEFT},

    {"ctrl", GLFW_KEY_LEFT_CONTROL},
    {"control", GLFW_KEY_LEFT_CONTROL},
    {"shift", GLFW_KEY_LEFT_SHIFT},
    {"alt", GLFW_KEY_LEFT_ALT},
    {"super", GLFW_KEY_LEFT_SUPER},

    {"lctrl", GLFW_KEY_LEFT_CONTROL},
    {"lshift", GLFW_KEY_LEFT_SHIFT},
    {"lalt", GLFW_KEY_LEFT_ALT},
    {"lgui", GLFW_KEY_LEFT_SUPER},
    {"lsuper", GLFW_KEY_LEFT_SUPER},

    {"rctrl", GLFW_KEY_RIGHT_CONTROL},
    {"rshift", GLFW_KEY_RIGHT_SHIFT},
    {"ralt", GLFW_KEY_RIGHT_ALT},
    {"rgui", GLFW_KEY_RIGHT_SUPER},
    {"rsuper", GLFW_KEY_RIGHT_SUPER},

    {"f1", GLFW_KEY_F1},
    {"f2", GLFW_KEY_F2},
    {"f3", GLFW_KEY_F3},
    {"f4", GLFW_KEY_F4},
    {"f5", GLFW_KEY_F5},
    {"f6", GLFW_KEY_F6},
    {"f7", GLFW_KEY_F7},
    {"f8", GLFW_KEY_F8},
    {"f9", GLFW_KEY_F9},
    {"f10", GLFW_KEY_F10},
    {"f11", GLFW_KEY_F11},
    {"f12", GLFW_KEY_F12},
};

} // namespace Artifex