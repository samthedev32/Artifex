#pragma once

#include <mathutil/common.hpp>

#include <cstdint>
#include <vector>

namespace Artifex {

class Engine;

// Mixer
struct Mix {
  public:
    // Initialize Mixer
    void init(Engine *pEngine);

    // DeInitialize Mixer
    void deinit();

  private:
    bool initialized = false;
    Engine *engine = nullptr;
};

} // namespace Artifex