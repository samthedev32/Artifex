#include <Artifex/core/Renderer.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <ctime>

using namespace Artifex;

float time() {
  timespec res{};
  clock_gettime(CLOCK_MONOTONIC, &res);
  return static_cast<float>(1000.0f * static_cast<float>(res.tv_sec) + static_cast<float>(res.tv_nsec) / 1e6) / 1000.0f;
}

int main() {
  Renderer renderer("Artifex Engine", {720, 480});

  int w, h, ch;
  stbi_set_flip_vertically_on_load(true);
  const unsigned char *data = stbi_load("../../../examples/gfx_simple/milk.png", &w, &h, &ch, 3);
  const uuid_t id = renderer.load_texture(data, {w, h}, ch);

  float past, now = time();
  while (renderer.update()) {
    past = now, now = time();
    float deltaTime = now - past;

    renderer.clear({});
    renderer.draw({}, {0.5}, time() / 2.0f, Renderer::TEXTURE, 0.4f, {}, id);

    if (renderer.window.key("esc"))
      renderer.window.exit();
  }

  return 0;
}