#include "Artifex/core/Load.hpp"
#include <Artifex/Engine.hpp>

using namespace Artifex;

int main() {
    Engine ax("Hello Artifex", 720, 480);

    ax.load.load("../../hello_artifex/milk.png");
    ax.load.music("../../hello_artifex/music.mp3");

    ax.mix.music(0, -1);

    float v = 0;
    while (ax.update({0.1f, 0.1f, 0.1f})) {
        ax.render.circle({}, 0.6f, 1);
        ax.render.circle({}, 0.5f, {1.0f, 0.4f, 0.0f});
        ax.render.circle({}, 0.3f, 1);
        // ax.render.rect(vec2(0.0f, 0.0f), vec2(1.6f, 1.5f), it.surface);
        // ax.render.rect(vec2(0.0f, 0.0f), vec2(1.0f, 0.8f), it.primary);
        // ax.render.rect(vec2(0.0f, 0.0f), vec2(0.5f, 0.4f), it.secondary);

        ax.render.circle(ax.cursor, 0.1f,
                         ax.key("left") ? (vec3){1.0f, 0.0f, 0.0f}
                                        : (vec3){0.0f, 1.0f, 0.0f});
    }

    return 0;
}