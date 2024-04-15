#include <Artifex/modules/Graphics.hpp>

namespace Artifex {

GraphicsModule::GraphicsModule(Engine &engine, uint32_t flags)
    : Module(engine, flags), window("Artifex Window", {}), renderer(window.size) {}

// Entity is created
bool GraphicsModule::onCreate() {
  // TODO: add entity data
  return true;
}

// Entity is destroyed
void GraphicsModule::onDestroy() {
  // TODO: delete entity data
}

// Entity os updated; render
void GraphicsModule::onUpdate(float deltaTime) {
  // TODO: draw entity
}

} // namespace Artifex