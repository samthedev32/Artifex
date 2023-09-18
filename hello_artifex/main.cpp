#include <Artifex/Engine.hpp>

using namespace Artifex;

class Game : public Module {
    bool onCreate() {
        ax->load.load("../../hello_artifex/milk.png");
        ax->load.music("../../hello_artifex/music.mp3");

        // ax->mix.music(0, -1);

        return true;
    }

    bool onUpdate(float deltaTime) {
        if (ax->key("esc"))
            ax->exit();

        // ax->render.circle({}, 0.6f, 1, ax->now * (45), 0, {1.0f, 0.2f});
        // ax->render.circle({}, 0.4f, 1, ax->now * 90, 0.7f);
        // ax->render.circle({}, 0.5f, {1.0f, 0.4f, 0.0f}, 0.8f);

        // ax->render.circle(ax->cursor, 0.1f,
        //                   ax->key("left") ? (vec3){1.0f, 0.0f, 0.0f}
        //                                   : (vec3){0.0f, 1.0f, 0.0f});

        ax->render.rounded(ax->cursor, 0.5f, 1, (sin(ax->now) + 1.0f) / 2.0f);

        return true;
    }
};

int main() {
    Engine ax("Hello Artifex", {720, 480});

    ax.add("game", new Game());

    ax.loop();

    return 0;
}