//
// Created by samthedev on 11/9/23.
//

#pragma once

#include <Artifex/types/entity.hpp>

#include <vector>

namespace Artifex {

class Engine;

// Module
class Module {
public:
  Engine &engine;

public:
  explicit Module(Engine &engine) : engine(engine) {}
  ~Module() = default;

  // Assigned Entity Created
  virtual bool onCreate(Entity &entity) { return true; }

  // Assigned Entity Destroyed
  virtual void onDestroy(Entity &entity) {}

  // Assigned Entity Updated
  virtual void onUpdate(Entity &entity, double deltaTime) {}

  // Global (Engine) Update; called per-frame
  virtual bool onGlobalUpdate(double deltaTime) { return false; }

  // Return Required Components
  virtual std::vector<std::pair<uuid_t, size_t>> ComponentList() { return {}; }
};

} // namespace Artifex