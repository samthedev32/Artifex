#include <Artifex/graphics/Renderer.hpp>
#include <Artifex/physics/Physics.hpp>

#include <ctime>

float time() {
  timespec res{};
  clock_gettime(CLOCK_MONOTONIC, &res);
  return static_cast<float>(1000.0f * static_cast<float>(res.tv_sec) + static_cast<float>(res.tv_nsec) / 1e6) / 1000.0f;
}

using namespace Artifex;

int main() {
  // Init Renderer
  Renderer renderer("Artifex || Physics", {720, 480});

  // Load Texture
  const unsigned char data[] = {255, 140, 0, 127}; // rgba
  const uuid_t tex = renderer.load_texture(data, {1, 1}, 4);

  // Init Physics
  Physics physics;

  float past, now = time();
  while (renderer.update()) {
    past = now, now = time();
    float deltaTime = now - past;

    renderer.clear({});
    //    graphics.draw({}, {0.5}, 0, Renderer::TEXTURE, 0.4f, {}, id);

    if (renderer.window.key("esc"))
      renderer.window.exit();
  }

  return 0;
}