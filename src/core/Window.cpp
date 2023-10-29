#include <Artifex/core/Window.hpp>

#include <SDL2/SDL.h>
#include <unordered_map>

namespace Artifex {

Window::Window(std::string name, vec<2, uint32_t> size)
    : size(size) {

  // Decide if Fullscreened or not
  bool isFullscreen = size->width == 0 || size->height == 0;
  size->width = size->width != 0 ? size->width : 1;
  size->height = size->height != 0 ? size->height : 1;

  // Init SDL2
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    Log::error("Window::Window", "Failed to Create Window: %s",
               SDL_GetError());
    return;
  }

  // Create Window
  window = SDL_CreateWindow(
      name.c_str(), SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED, size->width, size->height,
      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  if (window == NULL) {
    Log::error("Window::Window", "Failed to create window: %s",
               SDL_GetError());
    return;
  }

  fullscreen(isFullscreen);

  // Request OpenGL 3.3 or higher
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                      SDL_GL_CONTEXT_PROFILE_CORE);

  glcontext = SDL_GL_CreateContext(window);

  // Load GLAD
  if (!gladLoadGLLoader(SDL_GL_GetProcAddress)
      || glcontext == NULL) {
    Log::error("Window::Window", "Failed to init OpenGL");
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
  int w, h;
  SDL_GetWindowSize(window, &w, &h);
  size->width = w > 0 ? w : size->width;
  size->height = h > 0 ? h : size->height;

  // Update Window
  SDL_GL_SwapWindow(window);

  // update inputs
  keyboard = SDL_GetKeyboardState(NULL);

  vec<2, int> m;
  if (!SDL_GetRelativeMouseMode()) {
    SDL_GetMouseState(&m->x, &m->y);
    cursor->x = ((float)m->x / size->width) * 2 - 1;
    cursor->y = ((float)m->y / size->height) * -2 + 1;
  } else {
    SDL_GetRelativeMouseState(&m->x, &m->y);
    cursor->x = m->x * sensitivity;
    cursor->y = m->y * sensitivity;
  }

  // poll events
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      shouldClose = true;
      break;

    case SDL_KEYDOWN:
      if (event.key.keysym.scancode == SDL_SCANCODE_F11)
        fullscreen(!isFullscreen);
      break;

    case SDL_MOUSEBUTTONDOWN:
      if (0 < event.button.button && event.button.button < 4)
        mouse[event.button.button - 1]
            = true; // left, middle, right
      break;

    case SDL_MOUSEBUTTONUP:
      if (0 < event.button.button && event.button.button < 4)
        mouse[event.button.button - 1]
            = false; // left, middle, right
      break;

    case SDL_MOUSEWHEEL:
      if (event.wheel.y > 0) // scroll up
        scroll->y -= 1;
      else if (event.wheel.y < 0) // scroll down
        scroll->y += 1;

      if (event.wheel.x > 0) // scroll right
        scroll->x += 1;
      else if (event.wheel.x < 0) // scroll left
        scroll->x -= 1;
      break;

    default:
      break;
    }
  }

  return !shouldClose;
}

void Window::exit(bool sure) { shouldClose = sure; }

void Window::fullscreen(bool en, uint8_t hiddenCursor,
                        int minWidth, int minHeight) {
  SDL_SetWindowFullscreen(window,
                          SDL_WINDOW_FULLSCREEN_DESKTOP * en);

  bool cursor = hiddenCursor > true ? en : hiddenCursor;

  SDL_ShowCursor((SDL_bool)!cursor);
  SDL_SetRelativeMouseMode((SDL_bool)cursor);

  SDL_SetWindowMinimumSize(window, minWidth, minHeight);

  isFullscreen = en;
}

void Window::vsync(int interval) {
  SDL_GL_SetSwapInterval(interval);
}

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
  if (k == "ml" || k == "lmouse"
      || k == "left") // mouse button left
    return mouse[0];
  else if (k == "mm" || k == "mmouse"
           || k == "middle") // mouse button middle/scroll
    return mouse[1];
  else if (k == "mr" || k == "rmouse"
           || k == "right") // mouse button right
    return mouse[2];

  return keyboard[SDL2_SCANCODE_MAP[k]];
}

} // namespace Artifex