#include <Artifex/Engine.hpp>

#include <ctime>
#include <string>

namespace Artifex {

Engine::Engine(const std::string &name, const vec<2, uint32_t> &size) : Window(name, size) {
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

void Engine::loop(const vec<3> &clearColor, void (*onUpdate)(float)) {
  while (update(clearColor))
    if (onUpdate)
      onUpdate(deltaTime);
}

bool Engine::update(const vec<3> &clearColor) {
  // Update Screen
  bool running = Window::update();

  render.clear(clearColor);

  // Update GL
  glViewport(0, 0, (int)size->width, (int)size->height);

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
  struct timespec res {};
  clock_gettime(CLOCK_MONOTONIC, &res);
  return (float)(1000.0f * (float)res.tv_sec + (float)res.tv_nsec / 1e6) / 1000.0f;
}

void Engine::callback(void (*func)(int), void *userdata, int flags) {
  // TODO
}

} // namespace Artifex