#include <Artifex/Artifex.h>
#include <Artifex/load.h>

int main() {
    Artifex ax("Hello Artifex", 720, 480);

    // GL::LOAD::shader("shader/2d.glsl");

    while (ax.update()) {
        // TODO: Rendering

        ax.clear(1.0f, 0.4f, 0.0f);
    }

    return 0;
}