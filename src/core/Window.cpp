#include <Artifex/core/Window.hpp>

#include <unordered_map>

using namespace Artifex;

Window::Window(std::string name, int width, int height)
    : width(width), height(height) {

    // Decide if Fullscreened or not
    bool isFullscreen = width == 0 || height == 0;
    if (width <= 0 || height <= 0)
        width = 1, height = 1;

    // Init SDL2
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        log_error("Window::Window", "Failed to Create Window: %s",
                  SDL_GetError());
        return;
    }

    // Create Window
    window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, width, height,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE |
                                  SDL_WINDOW_FULLSCREEN_DESKTOP * isFullscreen);

    if (window == NULL) {
        log_error("Window::Window", "Failed to create window: %s",
                  SDL_GetError());
        return;
    }

    // Set Window's Minimum Size
    SDL_SetWindowMinimumSize(window, 720, 480);

    // Show/Hide Cursor (depending on fullscreen)
    SDL_ShowCursor((SDL_bool)!isFullscreen);
    SDL_SetRelativeMouseMode((SDL_bool)isFullscreen);

    // Init OpenGL 3.3 Core (or higher)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);

    glcontext = SDL_GL_CreateContext(window);

    // Load GLAD
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress) || glcontext == NULL) {
        log_error("Window::Window", "Failed to init OpenGL");
        SDL_DestroyWindow(window);
        return;
    }

    vsync(0);
}

Window::~Window() {
    if (window)
        SDL_DestroyWindow(window);
}

bool Window::update() {
    // Update Window Size
    SDL_GetWindowSize(window, &width, &height);

    // Update Window
    SDL_GL_SwapWindow(window);

    // Update Time
    past = now;
    now = time();
    deltaTime = now - past;

    // update inputs
    keyboard = SDL_GetKeyboardState(NULL);

    int m[2];
    if (!SDL_GetRelativeMouseMode()) {
        SDL_GetMouseState(&m[0], &m[1]);
        cursor.x = (m[0] / (float)width) * 2 - 1;
        cursor.y = (m[1] / (float)height) * -2 + 1;
        // cursor.x = map(m[0], 0, width, -1, 1);
        // cursor.y = map(m[1], 0, height, 1, -1);
    } else {
        SDL_GetRelativeMouseState(&m[0], &m[1]);
        cursor.x = m[0] * sensitivity;
        cursor.y = m[1] * sensitivity;
    }

    // poll events
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            shouldClose = true;
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (0 < event.button.button && event.button.button < 4)
                mouse[event.button.button - 1] = true; // left, middle, right
            break;

        case SDL_MOUSEBUTTONUP:
            if (0 < event.button.button && event.button.button < 4)
                mouse[event.button.button - 1] = false; // left, middle, right
            break;

        case SDL_MOUSEWHEEL:
            if (event.wheel.y > 0) // scroll up
                scroll.y -= 1;
            else if (event.wheel.y < 0) // scroll down
                scroll.y += 1;

            if (event.wheel.x > 0) // scroll right
                scroll.x += 1;
            else if (event.wheel.x < 0) // scroll left
                scroll.x -= 1;
            break;

        default:
            break;
        }
    }

    return !shouldClose;
}

void Window::exit(bool sure) { shouldClose = sure; }

void Window::fullscreen(bool en) {
    // if (en) {
    //     // Save Window Size
    //     small_size[0] = width;
    //     small_size[1] = height;

    //     // Get Monitor
    //     GLFWmonitor *monitor = glfwGetWindowMonitor(window);
    //     if (!monitor)
    //         monitor = glfwGetPrimaryMonitor();

    //     const GLFWvidmode *videoMode = glfwGetVideoMode(monitor);

    //     // Make Fullscreen
    //     glfwSetWindowMonitor(window, monitor, 0, 0, videoMode->width,
    //                          videoMode->height, GLFW_DONT_CARE);
    // } else {
    //     // Undo Fullscreen
    //     glfwSetWindowMonitor(window, nullptr, 0, 0, small_size[0],
    //                          small_size[1], GLFW_DONT_CARE);
    // }
}

void Window::vsync(int interval) { SDL_GL_SetSwapInterval(interval); }

std::unordered_map<std::string, int> SDL2_SCANCODE_MAP = {
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
    {"1", SDL_SCANCODE_1},
    {"2", SDL_SCANCODE_2},
    {"3", SDL_SCANCODE_3},
    {"4", SDL_SCANCODE_4},
    {"5", SDL_SCANCODE_5},
    {"6", SDL_SCANCODE_6},
    {"7", SDL_SCANCODE_7},
    {"8", SDL_SCANCODE_8},
    {"9", SDL_SCANCODE_9},
    {"0", SDL_SCANCODE_0},

    // Enter
    {"return", SDL_SCANCODE_RETURN},
    {"enter", SDL_SCANCODE_RETURN},
    {"\n", SDL_SCANCODE_RETURN},

    // Escape
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

bool Window::key(std::string k) {
    // Mouse Buttons
    if (k == "ml" || k == "lmouse" || k == "left") // mouse button left
        return mouse[0];
    else if (k == "mm" || k == "mmouse" ||
             k == "middle") // mouse button middle/scroll
        return mouse[1];
    else if (k == "mr" || k == "rmouse" || k == "right") // mouse button right
        return mouse[2];

    return keyboard[SDL2_SCANCODE_MAP[k]];
}