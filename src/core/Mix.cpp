#include <Artifex/core/Mix.hpp>

#include <Artifex/Engine.hpp>

#include <cstring>

using namespace Artifex;

void Mix::init(Engine *pEngine) {
    if (initialized)
        return;

    engine = pEngine;

    // Init

    initialized = true;
}

void Mix::deinit() {
    if (!initialized)
        return;

    // DeInit

    initialized = false;
}
