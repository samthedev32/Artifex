#pragma once

#include <Artifex/core/Mix.hpp>
#include <Artifex/core/Renderer.hpp>

#include <Artifex/types/types.hpp>

#include <Artifex/utility/uuid.h>

#include <cstdint>
#include <typeinfo>
#include <unordered_map>
#include <vector>

namespace Artifex {

class Engine {
public:
  Engine(const std::string &title, const vec<2, uint32_t> &size);
  ~Engine();

  // Game Loop
  void loop(const vec<3> &clearColor = {}, void (*onUpdate)(float) = nullptr);

  // Update Engine & Window (for manual game loop)
  bool update(const vec<3> &clearColor);

  // Get Current Time (s)
  static float time();

  // Get Ratio of Window (width/height)
  inline float ratio() { return (float)size->width / (float)size->height; }

  // Register Callback Function
  void callback(void (*func)(int), void *userdata, int flags);

  // Add Module
  template <typename T> bool add(const std::string &name, uint32_t flags = 0);

public:
  float now = 0.0f;
  float deltaTime = 0.0f;

  struct Entity {
    uuid_t texture, mesh, shader;
  };

  std::unordered_map<uuid_t, Entity> entities;

private:
  float past;

  std::unordered_map<std::string, Module *> module;
};

#include <type_traits>

template <typename T> bool Engine::add(const std::string &name, uint32_t flags) {
  static_assert(std::is_base_of<Module, T>::value, "Invalid Module");

  if (module.count(name)) {
    Log::warning("Engine::add", "Module with name '%s' already exists", name.c_str());
    return false;
  }

  module[name] = new T(*this, flags);

  if (!module[name]->onCreate()) {
    Log::warning("Engine::add"
                 "Failed to create module '%s'",
                 name.c_str());

    return false;
  }

  Log::verbose("Engine::add", "Added Module '%s'", name.c_str());
  return true;
}

} // namespace Artifex