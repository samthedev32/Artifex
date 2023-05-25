#include <Artifex/core/Window.h>

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
                scroll.y += 1;
            }
            else if (event.wheel.y < 0) // scroll down
            {
                scroll.y -= 1;
            }

            if (event.wheel.x > 0) // scroll right
            {
                scroll.x += 1;
            }
            else if (event.wheel.x < 0) // scroll left
            {
                scroll.x -= 1;
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
        cursor.x = (m[0] * 2.0f) / (float)size[0] - 1.0f;
        cursor.y = (m[1] * -2.0f) / (float)size[1] + 1.0f;
    }
    else
    {
        SDL_GetRelativeMouseState(&m[0], &m[1]);
        cursor.x = -m[1];
        cursor.y = m[0];
    }

    return !shouldClose;
}

/* Kill Window */
void Window::exit(bool sure)
{
    shouldClose = sure;
}

/*  ----  Keyboard & Mouse  ----  */

/* Get Key State */
bool Window::key(std::string k)
{
    // Lowercase Key Name
    k = string::lowercase(k);

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