#include <Artifex/renderer.hpp>
#include <Artifex/window.hpp>

/**
 * @brief Simple Rendering Example for C++
 */
int main() {
    Artifex::Window window1("Window 1", 720, 480);
    Artifex::Window window2("Window 2", 720, 480);

    Artifex::Renderer renderer1(window1);
    Artifex::Renderer renderer2(window2);

    while (window1.update() && window2.update()) {
        renderer1.update();
        renderer1.clear({1, 0.4f, 0});

        renderer2.update();
        renderer2.clear({1, 0, 1});
    }

    return 0;
}