#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

  /* Load Shader */ {
    FILE *f = fopen("../../../examples/hello_world/shader/2d.glsl", "r");

    if (f) {
      uint8_t current = 0;

      // RAW Shader Code (trash, vertex, fragment, geometry)
      std::string code[4];

      char line[256];
      while (fgets(line, sizeof(line), f)) {
        char index[10], parameter[10];
        sscanf(line, "%9s %9s", index, parameter);

        if (!strcmp(index, "#shader")) {
          if (!strcmp(parameter, "vertex"))
            current = 1;
          else if (!strcmp(parameter, "fragment"))
            current = 2;
          else if (!strcmp(parameter, "geometry"))
            current = 3;
          else
            Log::warning("Main/Load", "Invalid Shader type: %s", parameter);
        } else if (!strcmp(index, "#script")) {
          Log::warning("Main/Load", "Shader Scripts are not supported YET!\n");
        } else {
          code[current] += line;
        }
      }

      fclose(f);

      renderer.shader(code[1].c_str(), code[2].c_str(), code[3].c_str());
    } else {
      Log::error("Main/Load", "Failed to Open Shader File");
    }
  }

  /* Load Texture */ {
    vec<2, int> size;
    int ch;
    stbi_set_flip_vertically_on_load(true);
    void *image = stbi_load("../../../examples/hello_world/milk.png", &size->width, &size->height, &ch, 3);
    renderer.texture(image, size, ch);
  }

  while (renderer.update()) {
    Artifex::Renderer::clear({});

    renderer.roundable({}, {0.5f, 0.7f}, 0, {1.0f, 0.0f, 1.0f}, 1, 0.4f);
  }

  return 0;
}