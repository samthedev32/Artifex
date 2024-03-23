#define STB_IMAGE_IMPLEMENTATION

#include <Artifex/core/Log.hpp>
#include <Artifex/core/Renderer.hpp>
#include <cstring>

using namespace Artifex;
//
// class Game : public Artifex::Module {
// public:
//  Game(Engine &engine, uint32_t flags) : Artifex::Module(engine, ONCREATE | ONDESTROY | ONUPDATE) {}
//
//  bool onCreate() override {
//    engine.load.load("../../../examples/hello_world/milk.png");
//    engine.load.music("../../../examples/hello_world/music.mp3");
//
//    //    engine.mix.music(0, -1);
//
//    //    engine.callback(asd, ONCREATE);
//
//    return true;
//  }
//
//  // TODO: button style: half-rounded corners, rounded corners
//  // when hovered, shrink when clicked
//
//  bool button(const vec<2> &center, const vec<2> &size) {
//    int max = (engine.cursor->x + 1) / 2.0 * 1000;
//    float c = (engine.cursor->y + 1) / 2.0 * 30.0;
//
//    for (int i = 0; i < max; i++)
//      engine.render.roundable({-1 + i * 2.0 / max, 0.0}, {2.0 / max / 2.0, ((sin(engine.now + i * c / max) + 1) / 2) * 0.5},
//      0,
//                              vec<3>(1.0, 0.0, 1.0), 1, 0.5f, 90.0f);
//
//    printf("fps: %f\n", 1.0f / engine.deltaTime);
//    return false;
//  }
//
//  void onUpdate(float deltaTime) override { button({}, {0.4f, 0.2f}); }
//};

int main() {
  //  Engine ax("Hello, World!", {720, 480});
  //
  //  ax.add<Game>("game");
  //
  //  ax.loop({0.1f, 0.0f, 0.1f});

  Renderer renderer("Renderer Window", {720, 480});

  // renderer.load_shader("../../../examples/hello_world/shader/2d.glsl");
  // auto tex = renderer.load_texture("../../../examples/hello_world/milk.png");

  while (renderer.update()) {
    Artifex::Renderer::clear({});
    renderer.draw({}, {0.5}, 0, Artifex::Renderer::Look::COLOR, 0, {}, 0);

    // renderer.draw({1.0f, 0.0f, 1.0f});
    //    renderer.roundable({}, {0.5f, 0.7f}, 0, {1.0f, 0.0f, 1.0f}, 1, 0.4f);
  }

  return 0;
}