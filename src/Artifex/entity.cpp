#include <Artifex/Engine.hpp>

namespace Artifex {

uuid_t Engine::add(const EntityDescriptor &entityDescriptor) {
  // TODO
  Entity e;

  if (entityDescriptor.enable.renderer)
    e.modules.insert(m_graphics);

  //  if (entityDescriptor.defaultPhysics)
  //    e.modules.insert(physics);

  for (auto m : entityDescriptor.modules)
    e.modules.insert(m);

  uuid_t id = m_entity.add(e);

  for (auto m : m_entity[id].modules) {
    //    auto components = m_module[m]->ComponentList();
    //    for (auto [cid, type] : components)
    //      if (m_entity[id].components.count(cid) && m_entity[id].components[cid != type]) {
    //        Log::warning("Engine::add", "Entity component may be ... TODO");
    //      } else
    //        m_entity[id].components[cid] = type;

    m_module[m]->onCreate(m_entity[id]);
  }

  return id;
}

void Engine::removeEntity(uuid_t id) {
  for (auto m : m_entity[id].modules)
    m_module[m]->onDestroy(m_entity[id]);

  m_entity.erase(id);
}

Entity &Engine::getEntity(uuid_t id) { return m_entity.get(id); }

} // namespace Artifex