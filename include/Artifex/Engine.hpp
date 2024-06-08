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
   // namespace Properties {
   //    // Visual Properties of an Entity
   //    struct Visual {
   //       bool visible = false;
   //       std::string texture;
   //       vec<2> scale;
   //    };
   //
   //    // Physical Properties of an Entity
   //    struct Physical {
   //       bool collider = false;
   //       vec<2> offset, size;
   //
   //       bool is_static = false;
   //    };
   // }
   //
   // struct Scene
   // {
   //    std::unordered_map<ID, unsigned int> textuers;
   //    flecs::world ecs;
   // };

   class Engine
   {
   public:
      explicit Engine(const std::string& title, const vec<2, uint32_t>& size = {});

      ~Engine();

      // Scene Management
      void add();

      // Add Entity
//      flecs::entity add(const std::string& name, const vec<2>& center, const vec<2>& size);

      // TODO resource management & scene managing

      // Get Entity by name
//      flecs::entity get(const std::string& name);

      bool update();
      void loop(const std::function<void(float)>& callback = {});

      // private:
      Ref<Scene> scene;

      Window m_window;
      Mixer m_mixer;

      AssetManager m_asset;

      Renderer m_renderer;


      // Physics::Engine m_physics;
   };
}
