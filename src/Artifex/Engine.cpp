#include <Artifex/Engine.hpp>

#include <Artifex/modules/Graphics.hpp>

#include <ctime>

namespace Artifex {

Engine::Engine(const std::string &name, const vec<2, uint32_t> &size) {
  clock.reset();
  add<GraphicsModule>(); // TODO make it called for every entity
}

Engine::~Engine() {
  // Destroy Modules
  for (auto [_, m] : module) {
    m->onDestroy();
    delete m;
  }
}

void Engine::loop(const std::function<void(float)> &onUpdate) {
  clock.reset();
  while (update())
    if (onUpdate)
      onUpdate(clock.deltaTime);
}

bool Engine::update() {
  // Update Screen
  //  bool running = window.update();
  //  renderer.update();
  //  renderer.clear();

  // Timing
  clock.update();

  // Update Entities
  for (const auto &[id, e] : entity) {
    // Update Modules
    for (auto m : e.modules) {
      module[m]->onUpdate(clock.deltaTime);
    }
  }
  for (auto [_, m] : module)
    m->onUpdate(clock.deltaTime);

  //  return running;
  return true;
}

void Engine::callback(const std::function<void(int)> &func, void *userdata, int flags) {
  // TODO
}

uuid_t Engine::add(const EntityDescriptor &entityDescriptor) {
  // TODO
  return 0;
}

} // namespace Artifex