#include <Artifex/modules/Graphics.hpp>

namespace Artifex {

GraphicsModule::GraphicsModule(Engine &engine, uint32_t flags)
    : Module(engine, flags), window("Artifex Window", {720, 480}), renderer(window.size) {}

// Entity is created
bool GraphicsModule::onCreate(uuid_t id) {
  // TODO: add entity data
  return true;
}

// Entity is destroyed
void GraphicsModule::onDestroy(uuid_t id) {
  // TODO: delete entity data
}

// Entity os updated; render
void GraphicsModule::onUpdate(uuid_t id, float deltaTime) {
  renderer.draw({}, {0.1}, 0, Renderer::DYNAMIC, 0.6);
  // TODO: draw entity
}

bool GraphicsModule::onGlobalUpdate(float deltaTime) {
  bool running = window.update();

  renderer.update();
  renderer.clear();

  return running;
}

} // namespace Artifex