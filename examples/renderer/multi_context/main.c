#include <Artifex/log.h>
#include <Artifex/renderer.h>
#include <Artifex/window.h>
/**
 * @brief Rendering Example with multiple Windows
 */
int main() {
    axWindow window1, window2;
    axWindowCreate(&window1, "Window 1", 720, 480);
    axWindowCreate(&window2, "Window 2", 720, 480);

    ax_debug("main", "hello, world");

    axRenderer renderer1, renderer2;
    axRendererCreate(&renderer1, window1);
    axRendererCreate(&renderer2, window2);

    while (axWindowUpdate(window1) && axWindowUpdate(window2)) {
        axRendererUpdate(renderer1);
        axRendererClear(renderer1, (axColor){1, 0.4f, 0, 0});

        axRendererUpdate(renderer2);
        axRendererClear(renderer2, (axColor){1, 0, 1, 0});
    }

    axRendererDestroy(&renderer1);
    axWindowDestroy(&window1);

    axRendererDestroy(&renderer2);
    axWindowDestroy(&window2);
    return 0;
}