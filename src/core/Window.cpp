#include <Artifex/core/Window.hpp>

Window::Window(std::string name, uint width, uint height)
    : width(width), height(height) {

    // Decide if Fullscreened or not
    if (width == 0 || height == 0)
        width = 1, height = 1;

    small_size[0] = width, small_size[1] = height;

    // Init GLFW
    glfwInit();

    // Create Window
    window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

    // Exit if Window Creation Failed
    if (window == nullptr) {
        printf("ERROR: Failed to Create Window (%i)\n", glfwGetError(nullptr));
        glfwTerminate();
        throw "Window Creation Failed";
    }

    // Make OpenGL Context
    glfwMakeContextCurrent(window);

    // Init (Modern) OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Load OpenGL (exit if failed)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("ERROR: Failed to Load OpenGL (%u)\n", glGetError());
        glfwDestroyWindow(window);
        glfwTerminate();
        throw "OpenGL Load Failed";
    }

    // Set Window User Pointer to *this*
    glfwSetWindowUserPointer(window, this);

    // Set Callbacks
    glfwSetFramebufferSizeCallback(window, callback_resize);
    glfwSetKeyCallback(window, callback_key);
    glfwSetCursorPosCallback(window, callback_cursor);
    glfwSetScrollCallback(window, callback_scroll);

    vsync(0);
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool Window::update(float r, float g, float b) {
    // Update Window
    glfwSwapBuffers(window);

    glfwPollEvents();

    glfwGetFramebufferSize(window, &width, &height);

    return !glfwWindowShouldClose(window);
}

void Window::exit(bool sure) { glfwSetWindowShouldClose(window, sure); }

void Window::fullscreen(bool en) {
    if (en) {
        // Save Window Size
        small_size[0] = width;
        small_size[1] = height;

        // Get Monitor
        GLFWmonitor *monitor = glfwGetWindowMonitor(window);
        const GLFWvidmode *videoMode =
            glfwGetVideoMode(glfwGetPrimaryMonitor());

        // Make Fullscreen
        glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0,
                             videoMode->width, videoMode->height,
                             GLFW_DONT_CARE);

    } else {
        // Undo Fullscreen
        glfwSetWindowMonitor(window, nullptr, 0, 0, small_size[0],
                             small_size[1], GLFW_DONT_CARE);
    }
}

void Window::vsync(int interval) { glfwSwapInterval(interval); }

bool Window::key(std::string k) {
    return keyboard.count(glfwGetKeyScancode(GLFW_STRING_SCANCODE[k])) > 0;
}

std::map<std::string, int> GLFW_STRING_SCANCODE = {
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

void Window::callback_resize(GLFWwindow *window, int w, int h) {
    Window *self = (Window *)glfwGetWindowUserPointer(window);
    self->width = w;
    self->height = h;
}

void Window::callback_key(GLFWwindow *window, int key, int scancode, int action,
                          int mods) {
    Window *self = (Window *)glfwGetWindowUserPointer(window);

    if (action == GLFW_PRESS)
        self->keyboard.insert(scancode);
    else
        self->keyboard.erase(scancode);
}

void Window::callback_cursor(GLFWwindow *window, double x, double y) {
    Window *self = (Window *)glfwGetWindowUserPointer(window);

    self->cursor[0] = (x * 2.0f) / (double)self->width - 1.0f;
    self->cursor[1] = (y * -2.0f) / (double)self->height + 1.0f;
}

void Window::callback_scroll(GLFWwindow *window, double x, double y) {
    Window *self = (Window *)glfwGetWindowUserPointer(window);

    self->scroll[0] = x;
    self->scroll[1] = y;
}