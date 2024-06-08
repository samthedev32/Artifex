#include "Artifex/Log.hpp"
#include <Artifex/ECS/World.hpp>

namespace Artifex::ECS {

ID World::add(ID entity) {
  entity_index[entity] = {};

  entity_index[entity].archetype = 0;
  entity_index[entity].row = 0; // TODO

  return entity;
}

ID World::del(ID entity) {
  entity_index.erase(entity);

  // TODO cleanup

  return entity;
}

ID World::add(ID entity, ID component) {
  auto &record = entity_index[entity];

  auto &archetype = archetype_index.at(record.archetype);

  // Calculate desired archetype ID
  Type t = archetype.type;
  t.type.insert(component);
  ID id = t.id();

  // Check if archetype has edges
  if (archetype.edges.count(component) == 0) {
    // Calculate Type

    if (archetype_index.count(id) == 0) {
      // doesn't exist; create
      // TODO: columns, edges
      archetype_index.insert({id, {t, {}, {}}});
    }
  }

  // Check if edge has "add"
  if (archetype.edges[component].add == 0) {
    // Add to Edge
    archetype.edges[component].add = id;
  }

  // Set New Archetype ID
  ID newID = archetype.edges[component].add;

  if (newID != id) {
    // type mismatch; shouldn't happen
    // probably a problem in hashing
    Log::warning("ECS::World", "Archetype ID Mismatch");
  }

  entity_index[entity].archetype = newID;

  return entity;
}

bool World::del(ID entity, ID component) {
  auto &record = entity_index[entity];

  auto &archetype = archetype_index.at(record.archetype);

  // Calculate desired archetype ID
  Type t = archetype.type;
  t.type.erase(component);
  ID id = t.id();

  // Check if archetype has edges
  if (archetype.edges.count(component) == 0) {
    // Calculate Type

    if (archetype_index.count(id) == 0) {
      // doesn't exist; create
      // TODO: columns, edges
      archetype_index.insert({id, {t, {}, {}}});
    }
  }

  // Check if edge has "remove"
  if (archetype.edges[component].remove == 0) {
    // Add to Edge
    archetype.edges[component].remove = id;
  }

  // Set New Archetype ID
  ID newID = archetype.edges[component].remove;

  if (newID != id) {
    // type mismatch; shouldn't happen
    // probably a problem in hashing
    Log::warning("ECS::World", "Archetype ID Mismatch");
  }

  entity_index[entity].archetype = newID;

  return entity;
}

}