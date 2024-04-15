#pragma once

#include <Artifex/types/module.hpp>

#include <Artifex/graphics/Renderer.hpp>
#include <Artifex/graphics/Window.hpp>

namespace Artifex {

// Graphics Module
class GraphicsModule : public Module {
public:
  // Init Window & Renderer
  explicit GraphicsModule(Engine &engine, uint32_t flags);

  // Release Renderer & Destroy Window
  ~GraphicsModule() = default;

  // Entity is created
  bool onCreate() override;

  // Entity is destroyed
  void onDestroy() override;

  // Entity os updated; render
  void onUpdate(float deltaTime) override;

private:
  Window window;
  Renderer renderer;
};

} // namespace Artifex