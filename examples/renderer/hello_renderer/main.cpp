#include <Artifex/renderer.hpp>
#include <Artifex/window.hpp>

/**
 * @brief Simple Rendering Example for C++
 */
int main() {
    Artifex::Window window("Test Window", 720, 480);
    Artifex::Renderer renderer(window);

    while (window.update()) {
        renderer.update();
        renderer.clear({});

        if (window.getKeyState(Artifex::Window::Key::ESCAPE))
            window.exit();

        Artifex::Renderer::DrawInfo drawInfo{};
        drawInfo.style = 0;
        drawInfo.size = 0.5f;

        drawInfo.corner[0] = 0.4f, drawInfo.corner[1] = 0.4f, drawInfo.corner[2] = 0.4f, drawInfo.corner[3] = 0.4f;

        renderer.draw(drawInfo);
    }

    return 0;
}