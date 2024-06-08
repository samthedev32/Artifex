#include <Artifex/Engine.hpp>

namespace Artifex {
    Engine::Engine(const std::string &title, const vec<2, uint32_t> &size) : m_window(title, size),
                                                                             m_mixer(m_asset, nullptr),
                                                                             m_asset(m_window, m_mixer),
                                                                             m_renderer(m_window, m_asset) {
        // m_ecs.system<Physics::Transform>()
        //      .each([this](Physics::Transform& t) { this->m_physics.process(t); });
        //
        // m_ecs.system<Visual, EntityData>()
        //      .each([this](Visual& v, EntityData& d) { this->m_renderer.render(v, d); });
    }

    Engine::~Engine() {
    }

//    flecs::entity Engine::add(const std::string &name, const vec<2> &center, const vec<2> &size) {
//        // TODO
//        return {};
//        // flecs::entity entity = m_ecs.entity(name.c_str());
//        // entity.set<EntityData>({center, size});
//        // entity.set(Physics::Transform{0, center, size, {1, 2}});
//        // entity.set(Visual{});
//        // return entity;
//    }
//
//    flecs::entity Engine::get(const std::string &name) {
//        return {};
//        // return m_ecs.entity(name.c_str());
//    }

    bool Engine::update() {
        // Update Entities
        // if (!m_ecs.progress())
        // {
        //     // err
        //     printf("Something went wrong\n");
        // }

        bool run = m_window.update();
        m_renderer.update(m_window.size, m_window.ratio);
        m_renderer.clear();
        return run;
    }

    void Engine::loop(const std::function<void(float)> &callback) {
        while (update()) {
            callback(0.0f);
        }
    }
}
