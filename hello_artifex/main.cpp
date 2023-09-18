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

        // TODO: button style: half-rounded corners, rounded corners when
        // hovered, shrink when clicked
        ax->render.rounded(ax->cursor, {0.4f, 0.2f}, 1,
                           (sin(ax->now) + 1.0f) / 2.0f, 0.0f);

        return true;
    }
};

int main() {
    Engine ax("Hello Artifex", {720, 480});

    ax.add("game", new Game());

    ax.loop({0.1f, 0.0f, 0.1f});

    return 0;
}