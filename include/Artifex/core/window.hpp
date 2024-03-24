#pragma once

// OS Handling
#if defined(__WIN32)
#elif defined(__EMSCRIPTEN__)
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <set>
#include <string>

#include <Artifex/math/vec.hpp>

namespace Artifex {

// Windowing Class for GLFW
class Window {
public:
  // Create Window
  Window(const std::string &name, vec<2, uint32_t> size);

  // Destroy Window
  ~Window();

  // Update Window
  bool update();

  // Close Window
  void exit(bool sure = true) const;

  // Enable/Disable Fullscreen Mode
  void fullscreen(bool en = true, uint8_t hiddenCursor = 2, int minWidth = 720, int minHeight = 480);

  // Enable/Disable VSync
  static void vsync(int interval = 1);

  // Get Key State
  [[nodiscard]] bool key(const std::string &k) const;

  vec<2, uint32_t> size;
  float ratio;

  // Cursor & Scroll Positions
  vec<2, double> cursor, scroll;

protected:
  bool isFullscreen = false;

private:
  static void callback_resize(GLFWwindow *window, int w, int h);
  static void callback_key(GLFWwindow *window, int key, int scancode, int action, int mods);
  static void callback_cursor(GLFWwindow *window, double x, double y);
  static void callback_scroll(GLFWwindow *window, double x, double y);

  GLFWwindow *window = nullptr;
  vec<2, uint32_t> smallSize;

  std::set<int> keyboard;

  bool mouseState[3]{};
};

} // namespace Artifex