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

// Select OpenGL Version
#ifndef GL_MAJOR
#define GL_MAJOR 3
#endif

#ifndef GL_MINOR
#ifdef __pc
#define GL_MINOR 3
#else
#define GL_MINOR 0
#endif
#endif

#ifndef GL_PROFILE
#ifdef __pc
#define GL_PROFILE SDL_GL_CONTEXT_PROFILE_CORE
#else
#define GL_PROFILE SDL_GL_CONTEXT_PROFILE_ES
#endif
#endif

// Other Options
#ifndef SWAP_INTERVAL
#define SWAP_INTERVAL 1
#endif

#ifndef MIN_WIDTH
#define MIN_WIDTH 320
#endif

#ifndef MIN_HEIGHT
#define MIN_HEIGHT 240
#endif

#include <Artifex/core/load.h>

#include <GLFW/glfw3.h>

// OpenGL Windowing Class
class Window
{
private:
    GLFWwindow *window = nullptr;
    GLFWmonitor *monitor = nullptr;

    const Uint8 *keyboardState;
    static std::map<std::string, int> __keycode;

    bool mouseState[3];

public:
    int size[2];
    Window(std::string name, bool fullscreen = true, int w = 720, int h = 480);
    ~Window();

    bool update(float r = 0.0f, float g = 0.0f, float b = 0.0f);
    void exit(bool sure = true);

    double cursor[2];
    long scroll[2];

    bool key(std::string k);
};