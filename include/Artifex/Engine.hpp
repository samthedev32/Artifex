#pragma once

// #include <Artifex/Physics.hpp>

#include <Artifex/Window.hpp>
#include <Artifex/Renderer.hpp>

#include <Artifex/Mixer.hpp>

#include <Artifex/AssetManager.hpp>

#include <Artifex/types/Ref.hpp>

#include <functional>

namespace Artifex
{
   class Engine
   {
   public:
      explicit Engine(const std::string& title, const vec<2, uint32_t>& size = {});

      ~Engine();

      bool update();
      void loop(const std::function<void(float)>& callback = {});

      // private:
      Ref<Scene> scene;

      Window m_window;
      Renderer m_renderer;

      Mixer m_mixer;

      AssetManager m_asset;

      // Physics::Engine m_physics;
   };
}
