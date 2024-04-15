#pragma once

#include <Artifex/audio/Mix.hpp>

#include <Artifex/types/module.hpp>

#include <Artifex/utility/Clock.hpp>
#include <Artifex/utility/ComponentMap.hpp>
#include <Artifex/utility/Log.hpp>

#include <Artifex/math/vec.hpp>

#include <functional>

namespace Artifex {

struct Entity {
  // Entity Modules
  std::vector<uuid_t> modules;
};

class Engine {
public:
  explicit Engine(const std::string &title, const vec<2, uint32_t> &size = {});
  ~Engine();

  // Game Loop
  void loop(const std::function<void(float)> &onUpdate = {});

  // Update Engine & Window (for manual game loop)
  bool update();

  // Register Callback Function
  void callback(const std::function<void(int)> &func, void *userdata, int flags);

  // Add Module
  template <typename T> uuid_t add(uint32_t flags = 0);

  // Entity Descriptor
  struct EntityDescriptor {
    std::string texture;
  };

  // Add Entity
  uuid_t add(const EntityDescriptor &entityDescriptor);

  //  uuid_t queryModule(std::string name);
  //  Module &getModule(uuid_t module);

  //  uuid_t queryEntity(std::string name);
  //  Entity &getEntity(uuid_t entity);

private:
  Clock clock;

  ComponentMap<Module *> module;
  ComponentMap<Entity> entity;

  uuid_t graphics;

  //  std::unordered_map<std::string, uuid_t> moduleMap;
  //  std::unordered_map<std::string, uuid_t> entityMap;
};

#include <type_traits>

template <typename T> uuid_t Engine::add(uint32_t flags) {
  static_assert(std::is_base_of<Module, T>::value, "Invalid Module");

  uuid_t id = module.add(new T(*this, flags));

  if (!id || !module[id]->onCreate(0)) {
    Log::warning("Engine::add", "Failed to create module");

    return 0;
  }

  Log::verbose("Engine::add", "Added Module");
  return id;
}

} // namespace Artifex