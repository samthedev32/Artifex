#include <Artifex/Engine.hpp>

#include <stdio.h>
#include <string.h>
#include <string>
#include <time.h>

using namespace Artifex;

Engine::Engine(std::string name, ivec2 size) : Window(name, size) {
    load.init(this);

    render.init(this);
    ui.init(this);

    mix.init(this);

    past = now = time();
}

Engine::~Engine() {
    // Destroy Modules
    for (auto [_, m] : module) {
        m->onDestroy();
        delete m;
    }

    mix.deinit();

    ui.deinit();
    render.deinit();

    load.deinit();
}

void Engine::loop(EngineToolkit::vec<3> clearColor, bool (*onUpdate)(float)) {
    while (update(clearColor))
        if (onUpdate)
            if (!onUpdate(deltaTime))
                exit(true);
}

bool Engine::update(EngineToolkit::vec<3> clearColor) {
    // Update Screen
    bool running = Window::update();

    render.clear(clearColor);

    // Update GL
    glViewport(0, 0, size->x, size->y);

    // Timing
    past = now;
    now = time();
    deltaTime = now - past;

    // Update Modules
    for (auto [_, m] : module)
        m->onUpdate(deltaTime);

    return running;
}

float Engine::time() {
    struct timespec res;
    clock_gettime(CLOCK_MONOTONIC, &res);
    return (1000.0f * res.tv_sec + (float)res.tv_nsec / 1e6) / 1000.0f;
}

bool Engine::add(std::string name, Module *mod, bool enable) {
    if (module.count(name)) {
        EngineToolkit::Log::warning("Engine::add",
                                    "Module with name '%s' already exists",
                                    name.c_str());
        return false;
    }

    if (!mod) {
        EngineToolkit::Log::warning("Engine::add", "Module is not initialized");
        return false;
    }

    module[name] = mod;
    module[name]->ax = this;

    if (!module[name]->onCreate()) {
        EngineToolkit::Log::warning("Engine::add"
                                    "Failed to create module '%s'",
                                    name.c_str());

        return false;
    }

    EngineToolkit::Log::verbose("Engine::add", "Added Module '%s'",
                                name.c_str());
    return true;
}