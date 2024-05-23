#include <Artifex/modules/Graphics.hpp>

namespace Artifex {

GraphicsModule::GraphicsModule(Engine &engine, const char *title, const vec<2, uint32_t> &size)
    : Module(engine), window(title, size), renderer(window.size) {}

// Entity is created
bool GraphicsModule::onCreate(Entity &entity) {
  // TODO: add entity data
  return true;
}

// Entity is destroyed
void GraphicsModule::onDestroy(Entity &entity) {
  // TODO: delete entity data
}

// Entity os updated; render
void GraphicsModule::onUpdate(Entity &entity, double deltaTime) {
  auto center = entity.get<vec<2>>(0);
  auto size = entity.get<vec<2>>(1);
  renderer.draw(center, size, 0, Renderer::DYNAMIC, 0.6);
  // TODO: draw entity
}

bool GraphicsModule::onGlobalUpdate(double deltaTime) {
  bool running = window.update();

  renderer.update();
  renderer.clear();

  return running;
}

std::vector<std::pair<uuid_t, size_t>> GraphicsModule::ComponentList() {
  // Graphics Module requires position & size
  return {{0, typeid(vec<2>).hash_code()}, {1, typeid(vec<2>).hash_code()}};
}

} // namespace Artifex