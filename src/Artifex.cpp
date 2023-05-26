#include <Artifex/Artifex.h>

// Public

Artifex::Artifex(std::string name, uint width, uint height)
    : Window(name, width, height) {}

Artifex::~Artifex() {}

bool Artifex::update() { return Window::update(); }

void Artifex::clear(float red, float green, float blue, GLbitfield buffers) {
    glClearColor(red, green, blue, 1.0f);
    glClear(buffers);
}

// Private

float Artifex::ratio() { return (float)width / (float)height; }