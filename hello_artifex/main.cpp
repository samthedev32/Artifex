#include <Artifex/Artifex.hpp>

int main() {
    Artifex ax("Hello Artifex", 720, 480);

    ax.load.load("../../hello_artifex/milk.png");

    while (ax.update(1.0f, 0.4f, 0.0f)) {
        ax.render.rect(vec2(), vec2(1.0f, 1.0f), 1, ax.time() * 90);
        ax.render.circle(vec2(), 0.04f, vec3(1.0f, 0.4f, 0.0f));
    }

    return 0;
}