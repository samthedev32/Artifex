//
// Created by samthedev on 11/13/23.
//

#include <Artifex/ui/modifier.hpp>

namespace Artifex::UI {

Modifier::Modifier() = default;
Modifier::Modifier(const std::string &id) { this->load(id); }

Modifier Modifier::load(const std::string &id) {
  *this = modifiers[id];
  return *this;
}

Modifier Modifier::save(const std::string &id) {
  modifiers[id] = *this;
  return *this;
}

// TODO

} // namespace Artifex::UI
