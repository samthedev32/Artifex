#pragma once

#include "Artifex/utility/Log.hpp"
#include <Artifex/utility/ComponentMap.hpp>
#include <Artifex/utility/uuid.h>

#include <any>
#include <cstdlib>
#include <set>
#include <unordered_map>

// Entity Data
struct Entity {
  // List of Modules
  std::set<uuid_t> modules;

  ComponentMap<std::any> data;

  std::any &operator[](uuid_t id) { return data[id]; }

  // Get Entity Data
  template <typename T> T get(uuid_t id) {
    if (!data.has(id)) {
      Log::warning("Entity::get", "Entity component %i doesn't exist", id);
      return {};
    } else if (data[id].type().hash_code() != typeid(T).hash_code())
      Log::warning("Entity::get", "Entity component is the wrong type");

    return std::any_cast<T>(data[id]);
  }

  void set(uuid_t id, const std::any &value) {
    if (data.has(id) && data[id].type() != value.type())
      Log::warning("Entity::get", "Changing component type");

    data[id] = value;
  }
};