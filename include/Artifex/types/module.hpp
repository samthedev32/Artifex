//
// Created by samthedev on 11/9/23.
//

#pragma once

#include <Artifex/utility/uuid.h>

#include <cstdint>

namespace Artifex {

class Engine;

// Module
class Module {
public:
  Engine &engine;

public:
  explicit Module(Engine &engine, uint32_t flags) : engine(engine) {}
  ~Module() = default;

  // Assigned Entity Created
  virtual bool onCreate(uuid_t entity) { return true; }

  // Assigned Entity Destroyed
  virtual void onDestroy(uuid_t entity) {}

  // Assigned Entity Updated
  virtual void onUpdate(uuid_t entity, float deltaTime) {}

  // Global (Engine) Update; called per-frame
  virtual bool onGlobalUpdate(float deltaTime) { return false; }
};

} // namespace Artifex