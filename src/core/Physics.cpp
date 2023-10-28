#include <Artifex/core/Physics.hpp>

#include <Artifex/Engine.hpp>

namespace Artifex {

void Physics::init(Engine *pEngine) {
    if (initialized)
        return;

    engine = pEngine;

    // init

    initialized = true;
}

void Physics::deinit() {
    if (!initialized)
        return;

    // deinit

    initialized = false;
}

} // namespace Artifex