#include <Artifex/Engine.hpp>

using namespace Artifex;

class Game : public Module {
    bool onCreate() {
        ax->load.load("../../hello_artifex/milk.png");
        ax->load.music("../../hello_artifex/music.mp3");

        // ax->mix.music(0, -1);

        return true;
    }

    bool button(vec<2> center, vec<2> size) {
        ax->render.roundable(center, size, 2, vec<3>(), 1, 0.3f, 90.0f);
    }

    bool onUpdate(float deltaTime) {
        if (ax->key("esc"))
            ax->exit();

        // TODO: button style: half-rounded corners, rounded corners when
        // hovered, shrink when clicked

        ax->resource.shader[ax->current.shader].set("funi.time", ax->now);

        button({}, {0.4f, 0.2f});

        return true;
    }
};

int main() {
    Engine ax("Hello Artifex", {720, 480});

    ax.add("game", new Game());

    ax.loop({0.1f, 0.0f, 0.1f});

    return 0;
}