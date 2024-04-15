#include <Artifex/Engine.hpp>

#include <Artifex/modules/Graphics.hpp>

#include <ctime>

namespace Artifex {

Engine::Engine(const std::string &name, const vec<2, uint32_t> &size) {
  clock.reset();
  graphics = add<GraphicsModule>(); // TODO make it called for every entity
}

Engine::~Engine() {
  // Destroy Entities
  for (const auto &[id, e] : entity) {
    for (auto m : e.modules)
      module[m]->onDestroy(id);
  }
}

void Engine::loop(const std::function<void(float)> &onUpdate) {
  clock.reset();
  while (update())
    if (onUpdate)
      onUpdate(clock.deltaTime);
}

bool Engine::update() {
  // Timing
  clock.update();

  // Update Entities
  for (const auto &[id, e] : entity) {
    // Update Modules
    for (auto m : e.modules) {
      module[m]->onUpdate(id, clock.deltaTime);
    }
  }

  bool running = true;

  // Update Global Modules
  for (auto [_, m] : module) {
    if (!m->onGlobalUpdate(clock.deltaTime))
      running = false;
  }

  //  return running;
  return running;
}

void Engine::callback(const std::function<void(int)> &func, void *userdata, int flags) {
  // TODO
}

uuid_t Engine::add(const EntityDescriptor &entityDescriptor) {
  // TODO
  auto id = entity.add({});
  entity[id].modules.push_back(graphics);
  return id;
}

} // namespace Artifex