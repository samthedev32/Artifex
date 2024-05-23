#pragma once

#include <Artifex/types/entity.hpp>
#include <Artifex/types/module.hpp>

#include <Artifex/utility/Clock.hpp>
#include <Artifex/utility/ComponentMap.hpp>
#include <Artifex/utility/Log.hpp>

#include <Artifex/math/vec.hpp>

#include <Artifex/utility/ECS.hpp>

#include <any>
#include <functional>
#include <set>

namespace Artifex {

// Entity Descriptor
struct EntityDescriptor {
  // Modules to be linked to the entity
  std::set<uuid_t> modules;

  struct {
    // Enable Default Renderer
    bool renderer = true;

    // Enable Default Physics Engine
    bool physics = false;
  } enable;
};

class Engine {
public:
  explicit Engine(const char *title, const vec<2, uint32_t> &size = {});
  ~Engine();

  // Game Loop
  void loop(const std::function<void(double)> &onUpdate = {});

  // Update Engine (for manual game loop)
  bool update();

  // Module Manipulation
  template <typename T, typename... Args, typename = void> uuid_t add(Args... args);
  void removeModule(uuid_t id);
  Module &getModule(uuid_t id);

  // Entity Manipulation
  uuid_t add(const EntityDescriptor &entityDescriptor);
  void removeEntity(uuid_t id);
  Entity &getEntity(uuid_t id);

public:
  // TODO layers
  uuid_t m_graphics;

private:
  Clock m_clock;

  // Modules
  ComponentMap<Module *> m_module;

  // Entities
  ComponentMap<Entity> m_entity;

  ECS ecs;

  // TODO index everything by name
};

// Templated Implementations
#include "templated.hpp"

} // namespace Artifex