#pragma once

#include <set>
#include <string>

#include <Artifex/math/vec.hpp>

// GLFW Window
class GLFWwindow;

namespace Artifex {

// Windowing Class for OpenGL & Vulkan using GLFW with cross-platform support
class Window {
public:
  // Rendering API
  enum API { OPENGL, VULKAN, NONE };

  // Create Window
  Window(const std::string &name, vec<2, uint32_t> size, API api = OPENGL);

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