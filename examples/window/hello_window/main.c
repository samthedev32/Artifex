#include <Artifex/window.h>

/**
 * @brief Simple Windowing Example for C
 */
int main() {
    axWindow window;
    axWindowCreate(&window, "Test Window", 720, 480);

    while (axWindowUpdate(window))
        ;

    axWindowDestroy(&window);
    return 0;
}