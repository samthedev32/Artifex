#include <Artifex/ECS/World.hpp>
#include <cstdio>

using namespace Artifex;

int main() {
  // Create World
  ECS::World world;

  // Add new Entity
  if (world.add(0) != 0)
      printf("Failed to add entity\n");

  printf("Entity ID before: %zu\n", world.entity_index[0].archetype);

  world.add<int>(0);
  world.add<float>(0);

  printf("Entity ID after: %zu\n", world.entity_index[0].archetype);
  printf("Archetype Count: %zu\n", world.archetype_index.size());

  return 0;
}