#include <Artifex/Engine.hpp>

int main() {
  Artifex::Engine engine("Flappy Bird", {720, 480});

  engine.add({});

  engine.loop([&](float deltaTime) {
    // TODO
  });

  return 0;
}