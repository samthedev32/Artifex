#include "GL/load.h"
#include <Artifex/Artifex.h>

int main() {
    Artifex ax("Hello Artifex", 720, 480);

    GL::LOAD::shader("shader/2d.glsl");

    while (ax.update()) {
        // TODO: Rendering

        ax.clear(1.0f, 0.4f, 0.0f);
    }

    return 0;
}