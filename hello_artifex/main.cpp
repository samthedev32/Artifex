#include <Artifex/Artifex.h>

int main() {
    Artifex ax("Hello Artifex", 720, 480);

    // GL::LOAD::shader("shader/2d.glsl");
    ax.load("../../hello_artifex/shader/2d.glsl");

    while (ax.update(1.0f, 0.4f, 0.0f)) {
        // TODO: Rendering
    }

    return 0;
}