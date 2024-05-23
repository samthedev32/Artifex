#include <Artifex/Engine.hpp>

namespace Artifex {

void Engine::removeModule(uuid_t id) {
  // TODO
}

Module &Engine::getModule(uuid_t id) { return *m_module.get(id); }

} // namespace Artifex