#include <Artifex/renderer.h>
#include <Artifex/window.h>
#include <stdio.h>

#include "Artifex/keys.h"

/**
 * @brief Simple Rendering Example for C
 */
int main() {
    axWindow window;
    axWindowCreate(&window, "Test Window", 720, 480);

    axRenderer renderer;
    axRendererCreate(&renderer, window);

    int down = 0;
    while (axWindowUpdate(window)) {
        axRendererUpdate(renderer);
        axRendererClear(renderer, (axColor){});

        if (axWindowGetKeyState(window, AX_WINDOW_KEY_ESCAPE))
            axWindowExit(window);
        else if (axWindowGetKeyState(window, AX_WINDOW_KEY_F)) {
            if (!down) {
                axWindowSetFullscreen(window, !axWindowIsFullscreen(window), 0);
                down = 1;
                printf("asd\n");
            }
        } else {
            down = 0;
        }

        struct axRendererDrawInfo drawInfo = {};
        drawInfo.style = 0;
        drawInfo.size = (vec2){0.5f, 0.5f};

        drawInfo.corner[0] = 0.4f, drawInfo.corner[1] = 0.4f, drawInfo.corner[2] = 0.4f, drawInfo.corner[3] = 0.4f;

        axRendererDraw(renderer, &drawInfo);
    }

    axRendererDestroy(&renderer);
    axWindowDestroy(&window);
    return 0;
}