#include <Artifex/Engine.hpp>

#include <Artifex/modules/Graphics.hpp>

#include <type_traits>

namespace Artifex {

Engine::Engine(const char *title, const vec<2, uint32_t> &size) {
  m_clock.reset();
  m_graphics = add<GraphicsModule>(title, size);
}

Engine::~Engine() {
  // Destroy Entities
  for (const auto &[id, e] : m_entity) {
    for (auto m : e.modules)
      m_module[m]->onDestroy(m_entity[id]);
  }
}

void Engine::loop(const std::function<void(double)> &onUpdate) {
  m_clock.reset();
  while (update())
    if (onUpdate)
      onUpdate(m_clock.deltaTime);
}

bool Engine::update() {
  // Timing
  m_clock.update();

  // Update Entities
  for (const auto &[id, e] : m_entity) {
    // Update Modules
    for (auto m : e.modules) {
      m_module[m]->onUpdate(m_entity[id], m_clock.deltaTime);
    }
  }

  bool running = true;

  // Update Global Modules
  for (auto [_, m] : m_module) {
    if (!m->onGlobalUpdate(m_clock.deltaTime))
      running = false;
  }

  //  return running;
  return running;
}

} // namespace Artifex