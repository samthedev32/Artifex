#include <Artifex/Artifex.hpp>

int main() {
    Artifex ax("Hello Artifex", 720, 480);

    ax.load.load("../../hello_artifex/milk.png");

    while (ax.update(0.04f, 0.04f, 0.00f)) {
        ax.render.circle(vec2(), 0.4f, 1, ax.time() * 90.0f);
    }

    return 0;
}