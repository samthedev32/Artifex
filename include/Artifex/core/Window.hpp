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

#include <EngineToolkit/EngineToolkit.hpp>

#include <GL/glad.h>
#include <SDL2/SDL.h>

#include <string>

namespace Artifex {

using namespace EngineToolkit;

class Window {
  public:
    // Create Window
    Window(std::string name, vec<2, int> size);

    // Destroy Window
    ~Window();

    // Update Window
    bool update();

    // Close Window
    void exit(bool sure = true);

    // Enable/Disable Fullscreen Mode
    void fullscreen(bool en = true, uint8_t hiddenCursor = 2,
                    vec<2, int> minSize = {720, 480});

    // Enable/Disable VSync
    void vsync(int interval = 1);

    // Get Key State
    bool key(std::string k);

  public:
    // Width & Height
    vec<2, int> size;

    // Cursor & Scroll Positions
    vec<2> cursor, scroll;
    float sensitivity = 1.0f;

  private:
    SDL_Window *window = nullptr;
    SDL_GLContext glcontext;
    bool shouldClose = false;
    bool isFullscreen = false;

    const Uint8 *keyboard;
    bool mouse[3]{};
};

} // namespace Artifex