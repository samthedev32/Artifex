#pragma once

#include "uuid.h"

// Basic Entity Component System
struct ECS {
  template <typename T> void set(uuid_t id, T value);
  template <typename T> T get(uuid_t id);

  template <typename T> T &operator[](uuid_t id);
};