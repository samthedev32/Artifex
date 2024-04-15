#pragma once

#include <Artifex/math/vec.hpp>
#include <Artifex/utility/uuid.h>

#include <Artifex/utility/ComponentMap.hpp>

#include <unordered_map>

namespace Artifex {

// Basic 2D physics Engine
class Physics {
public: // Types
  // X by Y rectangle
  struct Rect {
    vec<2> center, size;
    vec<1> rotation;
  };

  // Internal Body
  struct Body {
    uuid_t center, size, velocity;
    uuid_t rotation, angularVelocity;
  };

public:
  // Add Body
  uuid_t add(const Rect &r);

  // Remove Body
  void remove(uuid_t id);

  // Update physics
  void update();

private:
  // Every Body
  ComponentMap<Body> body;

  // Components
  struct {
    ComponentMap<vec<1>> vec1; // 1D Vector / Float
    ComponentMap<vec<2>> vec2; // 2D Vector
  } comp;
};

} // namespace Artifex