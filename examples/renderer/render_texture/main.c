#include <Artifex/log.h>
#include <Artifex/renderer.h>
#include <Artifex/window.h>
#include <stb_image.h>

/**
 * @brief Simple Rendering Example for C
 */
int main() {
    axWindow window;
    axWindowCreate(&window, "Test Window", 720, 480);

    axRenderer renderer;
    axRendererCreate(&renderer, window);

    // Load Texture
    int w, h, ch;
    unsigned char* data = stbi_load("../../../../examples/resources/texture/milk.png", &w, &h, &ch, 0);

    if (!data) {
        printf("failed to load texture\n");
        return 1;
    }

    unsigned int tex = axRendererLoadTexture(renderer, w, h, ch, data);
    free(data);

    while (axWindowUpdate(window)) {
        axRendererUpdate(renderer);
        axRendererClear(renderer, (axColor){});

        if (axWindowGetKeyState(window, AX_WINDOW_KEY_ESCAPE))
            axWindowExit(window);

        struct axRendererDrawInfo drawInfo = {};
        drawInfo.style = 2;
        drawInfo.size = (axVector){0.5f, 0.5f};
        drawInfo.texture = tex;
        axWindowGetCursor(window, &drawInfo.center.x, &drawInfo.center.y);

        // drawInfo.corner[0] = 0.4f, drawInfo.corner[1] = 0.4f, drawInfo.corner[2] = 0.4f, drawInfo.corner[3] = 0.4f;
        drawInfo.corner[2] = 0.9f;

        axRendererDraw(renderer, &drawInfo);
    }

    axRendererDestroy(&renderer);
    axWindowDestroy(&window);
    return 0;
}