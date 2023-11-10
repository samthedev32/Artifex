// Windowing Library for Artifex
#pragma once

// Detect OS
#if defined(__linux)
#define __pc
#elif defined(__WIN32)
#define __windows
#define __pc

#define SDL_MAIN_HANDLED
#elif defined(__EMSCRIPTEN__)
#define __webassembly
#define __browser
#include <emscripten.h>
#include <emscripten/html5.h>
#else
#warning Targeted OS is not recognized
#endif

#include <GL/glad.h>
#include <SDL2/SDL.h>

#include <string>

#include <Artifex/core/Log.hpp>
#include <Artifex/math/vec.hpp>

namespace Artifex {

class Window {
public:
  // Create Window
  Window(const std::string& name, vec<2, uint32_t> size);

  // Destroy Window
  ~Window();

  // Update Window
  bool update();

  // Close Window
  void exit(bool sure = true);

  // Enable/Disable Fullscreen Mode
  void fullscreen(bool en = true, uint8_t hiddenCursor = 2,
                  int minWidth = 720, int minHeight = 480);

  // Enable/Disable VSync
  static void vsync(int interval = 1);

  // Get Key State
  bool key(const std::string& k);

public:
  vec<2, uint32_t> size;

  // Cursor & Scroll Positions
  vec<2> cursor, scroll;
  float sensitivity = 1.0f;

private:
  SDL_Window *window = nullptr;
  SDL_GLContext glcontext;
  bool shouldClose = false;
  bool isFullscreen = false;

  const Uint8 *keyboard = nullptr;
  bool mouse[3]{};
};

} // namespace Artifex