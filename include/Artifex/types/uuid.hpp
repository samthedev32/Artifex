#pragma once

#include <cstdint>

struct UUID {
  uint64_t id;

  static UUID nil();
  static UUID max();
};