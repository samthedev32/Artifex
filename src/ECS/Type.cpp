#include <Artifex/ECS/World.hpp>

namespace Artifex::ECS {

ID Type::id() {
  ID seed = type.size();
  for (auto &i : type) {
    seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  }
  return seed;
}

Type::operator ID() { return id(); }

}