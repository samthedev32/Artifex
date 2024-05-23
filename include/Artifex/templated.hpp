#pragma once

template <typename T, typename... Args, typename> uuid_t Engine::add(Args... args) {
  static_assert(std::is_base_of<Module, T>::value, "Invalid Module");

  uuid_t id = m_module.add(new T(*this, args...));

  if (!id) { // || !m_module[id]->onCreate()) {
    Log::warning("Engine::add", "Failed to create module");

    return 0;
  }

  // Register Components
  //  auto components = m_module[id]->ComponentList();
  //  for (auto [cid, type] : components) {
  //    if (componentType.count(cid)) {
  //
  //      if (componentType[cid] != type) {
  //        Log::warning("Engine::add", "Module has conflicting types");
  //      }
  //    } else {
  //      componentType[cid] = type;
  //    }
  //  }

  Log::verbose("Engine::add", "Added Module");
  return id;
}