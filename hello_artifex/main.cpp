#include <Artifex/Artifex.hpp>

int main() {
    Artifex ax("Hello Artifex", 720, 480);

    ax.load("../../overlayed.png");

    while (ax.update(1.0f, 0.4f, 0.0f)) {
        ax.rect(vec2(), vec2(1.0f, 1.0f), 1, ax.time() * 90);
    }

    return 0;
}