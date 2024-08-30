#include <stb_image.h>

#include <Artifex/renderer.hpp>
#include <Artifex/window.hpp>

/**
 * @brief Simple Rendering Example for C++
 */
int main() {
    Artifex::Window window("Test Window", 720, 480);
    Artifex::Renderer renderer(window);

    // Load Texture
    int w, h, ch;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* data = stbi_load("../../../../examples/resources/texture/milk.png", &w, &h, &ch, 0);

    if (!data) {
        printf("failed to load texture\n");
        return 1;
    }

    unsigned int tex = renderer.loadTexture(w, h, ch, data);
    free(data);

    while (window.update()) {
        renderer.update();
        renderer.clear({});

        if (window.getKeyState(Artifex::Window::Key::ESCAPE))
            window.exit();

        Artifex::Renderer::DrawInfo drawInfo{};
        drawInfo.style = 2;
        drawInfo.size = 0.5f;
        drawInfo.corner = 0.4f;
        drawInfo.texture = tex;

        renderer.draw(drawInfo);
    }

    return 0;
}
