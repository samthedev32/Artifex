#include <Artifex/core/Renderer.hpp>
#include <ctime>

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

float time() {
  timespec res{};
  clock_gettime(CLOCK_MONOTONIC, &res);
  return static_cast<float>(1000.0f * static_cast<float>(res.tv_sec) + static_cast<float>(res.tv_nsec) / 1e6) / 1000.0f;
}

int main() {
  //  Engine ax("Hello, World!", {720, 480});
  //
  //  ax.add<Game>("game");
  //
  //  ax.loop({0.1f, 0.0f, 0.1f});

  Renderer renderer("Artifex Engine", {720, 480});

  // renderer.load_shader("../../../examples/hello_world/shader/2d.glsl");
  // auto tex = renderer.load_texture("../../../examples/hello_world/milk.png");
  float past, now = time();
  while (renderer.update()) {
    past = now, now = time();
    float deltaTime = now - past;

    renderer.clear({});
    renderer.draw({}, {0.5}, time(), Artifex::Renderer::Look::DYNAMIC, 0.4f);

    if (renderer.window.key("f11"))
      renderer.window.fullscreen();
    if (renderer.window.key("f12"))
      renderer.window.fullscreen(false);
    if (renderer.window.key("esc"))
      renderer.window.exit();
  }

  return 0;
}