#include <Artifex/Engine.hpp>
#include <mathutil/log.hpp>

#include <stdio.h>
#include <string.h>
#include <string>
#include <time.h>

using namespace Artifex;

Engine::Engine(std::string name, int width, int height)
    : Window(name, width, height) {

    load.init(this);

    render.init(this);
    ui.init(this);

    mix.init(this);

    past = now = time();
}

Engine::~Engine() {
    mix.deinit();

    ui.deinit();
    render.deinit();

    load.deinit();
}

bool Engine::update(vec3 clearColor) {
    // Update Screen
    bool running = Window::update();

    render.clear(clearColor.r, clearColor.g, clearColor.b);

    // Update GL
    glViewport(0, 0, width, height);

    // Timing
    past = now;
    now = time();
    deltaTime = now - past;

    return running;
}

float Engine::time() {
    struct timespec res;
    clock_gettime(CLOCK_MONOTONIC, &res);
    return (1000.0f * res.tv_sec + (double)res.tv_nsec / 1e6) / 1000.0f;
}