#include <Artifex/window.hpp>

/**
 * @brief Simple Windowing Example for C++
 */
int main() {
    Artifex::Window window("Test Window", 720, 480);

    while (window.update())
        ;

    return 0;
}