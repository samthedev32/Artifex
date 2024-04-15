#include <Artifex/physics/Physics.hpp>

#include "collisions.cpp"

namespace Artifex {

// Add Body
uuid_t Physics::add(const Physics::Rect &r) {
  Body b{};
  b.center = comp.vec2.add(r.center);
  b.size = comp.vec2.add(r.size);
  b.rotation = comp.vec1.add(r.rotation);

  b.velocity = comp.vec2.add({});
  b.angularVelocity = comp.vec1.add({});

  return body.add(b);
}

// Remove Body
void Physics::remove(uuid_t id) { body.erase(id); }

void Physics::update() {
  for (auto [aKey, aRect] : body) {
    for (auto [bKey, bRect] : body) {
      if (aKey == bKey)
        continue;

      // TODO Collision Check
    }
  }
}

} // namespace Artifex