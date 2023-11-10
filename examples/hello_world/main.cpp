#include <Artifex/Engine.hpp>

using namespace Artifex;

class Game : public Artifex::Module {
public:
  explicit Game(Engine &engine) : Artifex::Module(engine, ONCREATE | ONDESTROY | ONUPDATE) {}

  bool onCreate() override {
    engine.load.load("../../../examples/hello_world/milk.png");
    engine.load.music("../../../examples/hello_world/music.mp3");

    // ax->mix.music(0, -1);

    //    engine.callback(asd, ONCREATE);

    return true;
  }

  // TODO: button style: half-rounded corners, rounded corners
  // when hovered, shrink when clicked

  bool button(const vec<2> &center, const vec<2> &size) {
    engine.render.roundable(center, size, 0, vec<3>(), 1, 0.3f, 90.0f);

    return false;
  }

  void onUpdate(float deltaTime) override { button({}, {0.4f, 0.2f}); }
};

int main() {
  Engine ax("Hello, World!", {720, 480});

  ax.add<Game>("game");

  ax.loop({0.1f, 0.0f, 0.1f});

  return 0;
}