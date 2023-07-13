#include <Artifex/Artifex.hpp>
#include <mathutil/log.hpp>

#include <stdio.h>
#include <string.h>
#include <string>
#include <time.h>

Artifex::Artifex(std::string name, int width, int height)
    : Window(name, width, height), load(this), render(this) {

    past = now = time();
}

Artifex::~Artifex() {}

bool Artifex::update(float r, float g, float b) {
    // Update Screen
    bool running = Window::update();

    // Clear Screen
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Update GL
    glViewport(0, 0, width, height);

    // Timing
    past = now;
    now = time();
    deltaTime = now - past;

    return running;
}

float Artifex::time() {
    struct timespec res;
    clock_gettime(CLOCK_MONOTONIC, &res);
    return (1000.0f * res.tv_sec + (double)res.tv_nsec / 1e6) / 1000.0f;
}

// Private

float Artifex::ratio() { return (float)width / (float)height; }