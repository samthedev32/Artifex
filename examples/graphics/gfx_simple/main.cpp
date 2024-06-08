#include <Artifex/Engine.hpp>

#include <ctime>

using namespace Artifex;

float time() {
  timespec res{};
  clock_gettime(CLOCK_MONOTONIC, &res);
  return static_cast<float>(1000.0f * static_cast<float>(res.tv_sec) + static_cast<float>(res.tv_nsec) / 1e6) / 1000.0f;
}

int main() {
  Engine engine("GFX Simple", {720, 480});

  // Load Image
  auto tex = engine.m_asset.load(AssetManager::Texture, "../../../../examples/resources/texture/milk.png");

  // Main Loop
  float past, now = time();
  while (engine.update()) {
    engine.m_renderer.image({}, {0.5}, tex, {0, 0.5f});
      engine.m_renderer.color({0.4, -0.4}, {0.2}, {}, {0, 1});

    if (engine.m_window.key("esc"))
      engine.m_window.exit();
  }

  return 0;
}