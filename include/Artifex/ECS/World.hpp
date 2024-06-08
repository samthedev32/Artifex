#pragma once

#include <typeinfo>
#include <unordered_map>

#include "Column.hpp"
#include "Archetype.hpp"

namespace Artifex::ECS {

struct Record {
    ID archetype;  // archetype of entity record
    ID row;
};

// Entity Ref
struct Entity {
};

// ECS World
struct World {
   public: // Entity Manipulation
    // Create New Entity
    ID add(ID entity);

    // Remove Entity
    ID del(ID entity);

    // Get Entity Reference
    //  Entity &get(ID entity);

   public: // Component Manipulation
    // Add Component to Entity
    ID add(ID entity, ID component);
    template <typename T>
    ID add(ID entity);

    // Remove Component from Entity
    bool del(ID entity, ID component);
    template <typename T>
    bool del(ID entity);

    // Get Component of entity
    //    void *get(ID entity, ID component);
    //    template <typename T> T &get(ID entity);

    // protected:
    std::unordered_map<ID, ArchetypeMap> component_index{};         // Map of which Archetypes contain a certain Component (and where)
    std::unordered_map<ID, Archetype> archetype_index = {{0, {}}};  // Map of what Archetype represents a Type

    std::unordered_map<ID, Record> entity_index{};

    //  template <typename T> bool has(ID entity);

    //  template <typename T> T &get(ID entity);

    //  Archetype &add_to_archetype(Archetype &src, ID id);
    //  void add_component(ID entity, ID component);
};

// ---- Templated Function Definitions

template <typename T>
ID World::add(ID entity) {
    return add(entity, typeid(T).hash_code());
}

template <typename T>
bool World::del(ID entity) {
    return del(entity, typeid(T).hash_code());
}

//
// template <typename T> bool World::has(ID entity) {
//  ID hash = typeid(T).hash_code();
//
//  const auto& archetype = entity_index[entity].archetype;
//  const auto &archetype_set = component_index[hash];
//  return archetype_set.count(archetype) != 0;
//}
//
// template <typename T> T &World::get(ID entity) {
//  ID hash = typeid(T).hash_code();
//
//  auto &record = entity_index[entity];
//  auto &archetype = record.archetype;
//
//  auto archetypes = component_index[hash];
//  if (archetypes.count(archetype) == 0)
//    return nullptr;
//
//  auto &a_record = archetypes[archetype];
//  return (T)archetype_index[archetype].columns[a_record.column][record.row];
//}

}  // namespace Artifex::ECS