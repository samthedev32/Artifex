#pragma once

#include <Artifex/types/module.hpp>

#include <Artifex/graphics/Renderer.hpp>
#include <Artifex/graphics/Window.hpp>

namespace Artifex {

// Graphics Module
class GraphicsModule : public Module {
public:
  // Init Window & Renderer
  GraphicsModule(Engine &engine, const char *title = "Artifex Engine", const vec<2, uint32_t> &size = {720, 480});

  // Release Renderer & Destroy Window
  ~GraphicsModule() = default;

  // Entity is created
  bool onCreate(Entity &entity) override;

  // Entity is destroyed
  void onDestroy(Entity &entity) override;

  // Entity is updated; render
  void onUpdate(Entity &entity, double deltaTime) override;

  // Update Window
  bool onGlobalUpdate(double deltaTime) override;

  // TODO: rename to allocateComponents
  std::vector<std::pair<uuid_t, size_t>> ComponentList() override;

private:
  Window window;
  Renderer renderer;
};

} // namespace Artifex