// A Basic Lightweight Windowing Library for Artifex
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

// Include OpenGL & SDL2
#include <GL/glad.h>
#include <SDL2/SDL.h>

#include <string>
#include <map>

// TODO: Audio Player

/* A Basic OpenGL Windowing Class */
class Window
{
private:
    // Window Management

    SDL_Window *window;
    SDL_GLContext context;
    bool shouldClose = false;

    // Keyboard & Mouse

    const Uint8 *keyboardState;
    static std::map<std::string, int> __keycode;

    bool mouseState[3];

public:
    // Window Management

    int size[2]; //< The {width, height} of the window
    Window(std::string name, bool fullscreen = true, int w = 720, int h = 480);
    ~Window();

    bool update(float r = 0.0f, float g = 0.0f, float b = 0.0f);
    void kill(bool sure = true);

    // Keyboard & Mouse

    float cursor[2]; //< The Cursor {x, y} Position
    int scroll[2];   //< The Scroll {x, y} Position
    bool key(std::string k);
};

/*  ----  Window Management  ----  */

/* Initalize Window */
Window::Window(std::string name, bool fullscreen, int w, int h) : size{w, h}
{
    // Init Windowing Library
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "window(%s); failed to initalize: %s\n", name.c_str(), SDL_GetError());
        exit(-1);
    }

    Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

// Add PC Flags (for fullscreen)
#if defined(__pc)
    flags |= (SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_INPUT_FOCUS) * fullscreen;
#endif

    // Create Window
    window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size[0], size[1], flags);
    if (window == NULL)
    {
        fprintf(stderr, "window(%s); failed to create window: %s\n", name.c_str(), SDL_GetError());
        exit(-1);
    }

    // Init OpenGL
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, GL_MAJOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, GL_MINOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, GL_PROFILE);

    context = SDL_GL_CreateContext(window);
    if (context == NULL)
    {
        fprintf(stderr, "window(%s); failed to create opengl context: %s\n", name.c_str(), SDL_GetError());
        exit(-1);
    }

    if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
        fprintf(stderr, "window(%s); failed to initalize opengl\n", name.c_str());
        exit(-1);
    }

    // Configure Window
    SDL_GL_SetSwapInterval(SWAP_INTERVAL); //< Set Swap Interval (default: 1 = vsync)

    SDL_SetWindowMinimumSize(window, MIN_WIDTH, MIN_HEIGHT); //< Set Min Window Size (default: 320 * 240)
    SDL_ShowCursor((SDL_bool)!fullscreen);                   //< Show/Hide Cursor (controlled by fullscreen)

    SDL_SetRelativeMouseMode((SDL_bool)fullscreen); //< Set Relative Cursor (controlled by fullscreen)
    SDL_GetWindowSize(window, &size[0], &size[1]);  //< Get Window Size
    keyboardState = SDL_GetKeyboardState(NULL);     //< Get Keyboard State
}

/* Destroy Window */
Window::~Window()
{
    SDL_DestroyWindow(window);
    SDL_Quit();
}

/* Update The Window */
bool Window::update(float r, float g, float b)
{
    // Update Window
    SDL_GL_SwapWindow(window);

    // Clear Screen
    int fb;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fb);
    if (fb != 0)
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Poll Events
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            shouldClose = true;
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
                mouseState[0] = true;
            if (event.button.button == SDL_BUTTON_RIGHT)
                mouseState[1] = true;
            if (event.button.button == SDL_BUTTON_MIDDLE)
                mouseState[2] = true;
            break;

        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT)
                mouseState[0] = false;
            if (event.button.button == SDL_BUTTON_RIGHT)
                mouseState[1] = false;
            if (event.button.button == SDL_BUTTON_MIDDLE)
                mouseState[2] = false;
            break;

        case SDL_MOUSEWHEEL:
            if (event.wheel.y > 0) // scroll up
            {
                scroll[1] += 1;
            }
            else if (event.wheel.y < 0) // scroll down
            {
                scroll[1] -= 1;
            }

            if (event.wheel.x > 0) // scroll right
            {
                scroll[0] += 1;
            }
            else if (event.wheel.x < 0) // scroll left
            {
                scroll[0] -= 1;
            }
            break;

        default:
            break;
        }
    }

    // Get Window Size
    SDL_GetWindowSize(window, &size[0], &size[1]);

    // Get Keyboard State
    keyboardState = SDL_GetKeyboardState(NULL);

    // Get Cursor Pos
    int m[2];
    if (!SDL_GetRelativeMouseMode())
    {
        SDL_GetMouseState(&m[0], &m[1]);
        cursor[0] = (m[0] * 2.0f) / (float)size[0] - 1.0f;
        cursor[1] = (m[1] * -2.0f) / (float)size[1] + 1.0f;
    }
    else
    {
        SDL_GetRelativeMouseState(&m[0], &m[1]);
        cursor[0] = -m[1];
        cursor[1] = m[0];
    }

    return !shouldClose;
}

/* Kill Window */
void Window::kill(bool sure)
{
    shouldClose = sure;
}

/*  ----  Keyboard & Mouse  ----  */

/* Get Key State */
bool Window::key(std::string k)
{
    // Lowercase Key Name
    for (int i = 0; i < (int)k.size(); i++)
        if (k[i] >= 'A' && k[i] <= 'Z')
            k[i] += 'a' - 'A';

    if (k == "lmouse" || k == "left")
        return mouseState[0];
    else if (k == "rmouse" || k == "right")
        return mouseState[1];
    else if (k == "mmouse" || k == "middle")
        return mouseState[2];
    else if (k == "mouse")
        return (key("left") || key("right"));

    if (__keycode.count(k) <= 0 || keyboardState == NULL)
        return false;

    return keyboardState[__keycode[k]];
}

// KeyCodes
std::map<std::string, int> Window::__keycode = {
    // Letters
    {"a", SDL_SCANCODE_A},
    {"b", SDL_SCANCODE_B},
    {"c", SDL_SCANCODE_C},
    {"d", SDL_SCANCODE_D},
    {"e", SDL_SCANCODE_E},
    {"f", SDL_SCANCODE_F},
    {"g", SDL_SCANCODE_G},
    {"h", SDL_SCANCODE_H},
    {"i", SDL_SCANCODE_I},
    {"j", SDL_SCANCODE_J},
    {"k", SDL_SCANCODE_K},
    {"l", SDL_SCANCODE_L},
    {"m", SDL_SCANCODE_M},
    {"n", SDL_SCANCODE_N},
    {"o", SDL_SCANCODE_O},
    {"p", SDL_SCANCODE_P},
    {"q", SDL_SCANCODE_Q},
    {"r", SDL_SCANCODE_R},
    {"s", SDL_SCANCODE_S},
    {"t", SDL_SCANCODE_T},
    {"u", SDL_SCANCODE_U},
    {"v", SDL_SCANCODE_V},
    {"w", SDL_SCANCODE_W},
    {"x", SDL_SCANCODE_X},
    {"y", SDL_SCANCODE_Y},
    {"z", SDL_SCANCODE_Z},

    // Numbers
    {"0", SDL_SCANCODE_0},
    {"1", SDL_SCANCODE_1},
    {"2", SDL_SCANCODE_2},
    {"3", SDL_SCANCODE_3},
    {"4", SDL_SCANCODE_4},
    {"5", SDL_SCANCODE_5},
    {"6", SDL_SCANCODE_6},
    {"7", SDL_SCANCODE_7},
    {"8", SDL_SCANCODE_8},
    {"9", SDL_SCANCODE_9},

    // Enter
    {"return", SDL_SCANCODE_RETURN},
    {"enter", SDL_SCANCODE_RETURN},
    {"\n", SDL_SCANCODE_RETURN},

    // Escape
    {"escape", SDL_SCANCODE_ESCAPE},
    {"esc", SDL_SCANCODE_ESCAPE},

    // Backspace
    {"backspace", SDL_SCANCODE_BACKSPACE},
    {"\b", SDL_SCANCODE_BACKSPACE},

    // Tab
    {"tab", SDL_SCANCODE_TAB},
    {"\t", SDL_SCANCODE_TAB},

    // Space
    {"space", SDL_SCANCODE_SPACE},
    {" ", SDL_SCANCODE_SPACE},

    // Control Keys
    {"lctrl", SDL_SCANCODE_LCTRL},
    {"lshift", SDL_SCANCODE_LSHIFT},
    {"lalt", SDL_SCANCODE_LALT},
    {"lgui", SDL_SCANCODE_LGUI},

    {"rctrl", SDL_SCANCODE_RCTRL},
    {"rshift", SDL_SCANCODE_RSHIFT},
    {"ralt", SDL_SCANCODE_RALT},
    {"rgui", SDL_SCANCODE_RGUI},

    // Function Keys
    {"f1", SDL_SCANCODE_F1},
    {"f2", SDL_SCANCODE_F2},
    {"f3", SDL_SCANCODE_F3},
    {"f4", SDL_SCANCODE_F4},
    {"f5", SDL_SCANCODE_F5},
    {"f6", SDL_SCANCODE_F6},
    {"f7", SDL_SCANCODE_F7},
    {"f8", SDL_SCANCODE_F8},
    {"f9", SDL_SCANCODE_F9},
    {"f10", SDL_SCANCODE_F10},
    {"f11", SDL_SCANCODE_F11},
    {"f12", SDL_SCANCODE_F12},
};
