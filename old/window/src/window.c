#include <Artifex/log.h>
#include <Artifex/window.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#define TAG "axWindow"

#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a > b ? a : b)

struct axWindow_ {
    int width, height;
    float ratio;

    GLFWwindow* window;

    int fullscreen;
    int smallPos[2], smallSize[2];  // pos & size when not fullscreen

    //    vec<2, uint32_t> smallSize;
    //
    //    std::set<int> keyboard;

    int mouseButton[3];

    float scroll[2];
};

int axWindowIsValid(axWindow window) {
    if (!window || !window->window)
        return 0;

    return 1;
}

static void ax_window_callback_resize(GLFWwindow* window, int w, int h);
static void ax_window_callback_scroll(GLFWwindow* window, double x, double y);
static GLFWmonitor* ax_window_get_monitor(GLFWwindow* window);

int axWindowCreate(axWindow* window, const char* title, int width, int height) {
    if (!window) {
        ax_error(TAG, "invalid window");
        return 1;
    }

    axWindow wnd = *window = malloc(sizeof(struct axWindow_));
    if (!wnd) {
        ax_error(TAG, "failed to allocate memory");
        return 1;
    }

    wnd->fullscreen = width == 0 || height == 0;

    if (wnd->fullscreen) {
        width = 1, height = 1;
    }

    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);

    // Create Window
    wnd->window = glfwCreateWindow(width, height, title, wnd->fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);

    // Exit if Window Creation Failed
    if (!wnd->window) {
        free(*window);
        *window = NULL;
        ax_error(TAG, "failed to create window");
        return 1;
    }

    // Set Window User Pointer
    glfwSetWindowUserPointer(wnd->window, *window);

    // Set Callbacks
    glfwSetFramebufferSizeCallback(wnd->window, ax_window_callback_resize);
    glfwSetScrollCallback(wnd->window, ax_window_callback_scroll);

    ax_verbose(TAG, "initialized window");
    return 0;
}

void axWindowDestroy(axWindow* window) {
    if (!window || !axWindowIsValid(*window)) {
        ax_warning(TAG, "invalid window");
        return;
    }

    glfwDestroyWindow((*window)->window);

    free(*window);
    *window = NULL;

    ax_verbose(TAG, "destroyed window");
}

int axWindowUpdate(axWindow window) {
    if (!axWindowIsValid(window)) {
        ax_error(TAG, "invalid window");
        return 0;
    }

    glfwSwapBuffers(window->window);

    glfwPollEvents();

    return !glfwWindowShouldClose(window->window);
}

void axWindowExit(axWindow window) {
    if (!axWindowIsValid(window)) {
        ax_warning(TAG, "invalid window");
        return;
    }

    ax_verbose(TAG, "exiting");
    glfwSetWindowShouldClose(window->window, 1);
}

void axWindowPos(axWindow window, int* x, int* y) {
    if (!axWindowIsValid(window)) {
        ax_error(TAG, "invalid window");
        return;
    }

    glfwGetWindowPos(window->window, x, y);
}

void axWindowSize(axWindow window, int* width, int* height) {
    if (!axWindowIsValid(window)) {
        ax_error(TAG, "invalid window");
        return;
    }

    glfwGetWindowSize(window->window, width, height);
}

float axWindowRatio(axWindow window) {
    if (!axWindowIsValid(window)) {
        ax_error(TAG, "invalid window");
        return 1;
    }

    int width = 0, height = 0;
    axWindowSize(window, &width, &height);
    return (float)width / (float)height;
}

void axWindowSetFullscreen(axWindow window, int en, int hideCursor) {
    if (!axWindowIsValid(window)) {
        ax_warning(TAG, "invalid window");
        return;
    }

    if (en) {
        // Save Window Size
        // smallSize = size;
        axWindowPos(window, &window->smallPos[0], &window->smallPos[1]);
        axWindowSize(window, &window->smallSize[0], &window->smallSize[1]);

        // Get Monitor
        GLFWmonitor* monitor = ax_window_get_monitor(window->window);
        const GLFWvidmode* videoMode = glfwGetVideoMode(monitor != NULL ? monitor : glfwGetPrimaryMonitor());

        // Make Fullscreen
        glfwSetWindowMonitor(window->window, monitor != NULL ? monitor : glfwGetPrimaryMonitor(), 0, 0, videoMode->width, videoMode->height,
                             GLFW_DONT_CARE);

        ax_verbose(TAG, "window is now fullscreen");
        window->fullscreen = 1;
    } else {
        // Undo Fullscreen
        glfwSetWindowMonitor(window->window, NULL, window->smallPos[0], window->smallPos[1], window->smallSize[0], window->smallSize[1], GLFW_DONT_CARE);

        ax_verbose(TAG, "windows is no longer fullscreen");
        window->fullscreen = 0;
    }
}

int axWindowIsFullscreen(axWindow window) {
    if (!axWindowIsValid(window)) {
        ax_warning(TAG, "invalid window");
        return 0;
    }

    return window->fullscreen;
}

void axWindowSetVSync(axWindow window, int interval) {
    if (!axWindowIsValid(window)) {
        ax_warning(TAG, "invalid window");
        return;
    }

    axWindowMakeCurrent(window);

    glfwSwapInterval(interval);

    ax_verbose(TAG, "set vsync to %i", interval);
}

int axWindowGetCursor(axWindow window, float* x, float* y) {
    if (!axWindowIsValid(window)) {
        ax_error(TAG, "invalid window");
        return 1;
    }

    double xx, yy;
    int w, h;
    axWindowSize(window, &w, &h);
    glfwGetCursorPos(window->window, &xx, &yy);
    *x = ((xx / (float)w) * 2) - 1, *y = -(((yy / (float)h) * 2) - 1);
    return 0;
}

// int axWindowGetScroll(axWindow window, float* x, float* y) {
//     if (!axWindowIsValid(window))
//         return 1;
//     return 1;
// }

int axWindowGetKeyState(axWindow window, enum axWindowKey key) {
    if (!axWindowIsValid(window)) {
        ax_error(TAG, "invalid window");
        return 0;
    }

    if (0 <= key && key < 360)
        return glfwGetKey(window->window, key);
    else if (360 <= key && key < 368)
        return glfwGetMouseButton(window->window, key - 360);

    return 0;
}

axWindowProcAddr axWindowGetAddress(const char* procname) {
    return glfwGetProcAddress(procname);
}

void axWindowMakeCurrent(axWindow window) {
    if (!axWindowIsValid(window)) {
        ax_warning(TAG, "invalid window");
        return;
    }

    glfwMakeContextCurrent(window->window);
}

// ---- misc

void ax_window_callback_resize(GLFWwindow* window, int w, int h) {
    axWindow wnd = glfwGetWindowUserPointer(window);
    wnd->width = w, wnd->height = h, wnd->ratio = (float)w / (float)h;
}

void ax_window_callback_scroll(GLFWwindow* window, double x, double y) {
    // TODO
}

GLFWmonitor* ax_window_get_monitor(GLFWwindow* window) {
    // temporarily stolen from
    // https://stackoverflow.com/questions/21421074/how-to-create-a-full-screen-window-on-the-current-monitor-with-glfw
    // since this library will be rewritten using X11/Wayland/WinAPI

    int nmonitors, i;
    int wx, wy, ww, wh;
    int mx, my, mw, mh;
    int overlap, bestoverlap;
    GLFWmonitor* bestmonitor;
    GLFWmonitor** monitors;
    const GLFWvidmode* mode;

    bestoverlap = 0;
    bestmonitor = NULL;

    glfwGetWindowPos(window, &wx, &wy);
    glfwGetWindowSize(window, &ww, &wh);
    monitors = glfwGetMonitors(&nmonitors);

    for (i = 0; i < nmonitors; i++) {
        mode = glfwGetVideoMode(monitors[i]);
        glfwGetMonitorPos(monitors[i], &mx, &my);
        mw = mode->width;
        mh = mode->height;

        overlap =
            MAX(0, MIN(wx + ww, mx + mw) - MAX(wx, mx)) *
            MAX(0, MIN(wy + wh, my + mh) - MAX(wy, my));

        if (bestoverlap < overlap) {
            bestoverlap = overlap;
            bestmonitor = monitors[i];
        }
    }

    return bestmonitor;
}