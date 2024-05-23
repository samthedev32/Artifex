#include <Artifex/Engine.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "../../helper/stb_image.h"

#include <string>

int main() {
  Artifex::Engine engine("Flappy Bird", {720, 480});

  uuid_t id = engine.add({});

  Entity &e = engine.getEntity(id);

  e[0] = vec<2>(0);
  e[1] = vec<2>(0.5, 0.3);

  auto pos = e.get<vec<2>>(0);

  // Set Position & Size for Entity [id]
  //  e.set(0, vec<2>{});
  //  e.set(1, vec<2>{0.5, 0.3});

  // Create Entity
  const std::string a = "../../../../examples/games/flappy_bird/";
  const std::string path[] = {"bird.png", "pipe.png"};
  const size_t textures = sizeof(path) / sizeof(*path);

  //  uuid_t texture[textures];
  //  for (int i = 0; i < textures; i++) {
  //    int w, h, ch;
  //    stbi_set_flip_vertically_on_load(true);
  //    const unsigned char *data = stbi_load((a + path[i]).c_str(), &w, &h, &ch, 3);
  //    texture[i] = engine.getModule(engine.m_graphics).load_texture(data, {w, h}, ch);
  //  }

  engine.loop([&](float deltaTime) {
    // game loop
  });

  return 0;
}