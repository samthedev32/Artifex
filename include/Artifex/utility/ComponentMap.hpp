#pragma once

#include "uuid.h"

#include <unordered_map>

// Unordered UUID - T Map
template <typename T> class ComponentMap {
public:
  //  ComponentMap();
  ~ComponentMap() { data.clear(); }

  // Add Element
  uuid_t add(T t);

  // Get Element
  T &get(uuid_t id);
  T &operator[](uuid_t id);

  // Erase Element
  bool erase(uuid_t id);

  // Check if UUID exists
  bool has(uuid_t id);

  auto begin() { return data.begin(); }
  auto end() { return data.end(); }

private:
  std::unordered_map<uuid_t, T> data;
  T dummy;
};

template <typename T> uuid_t ComponentMap<T>::add(T t) {
  uuid_t id;

  // Make sure we do not have it already
  // TODO optimize
  do {
    id = uuid_generate();
  } while (has(id));

  data[id] = t;
  return id;
}

template <typename T> T &ComponentMap<T>::get(uuid_t id) {
  if (data.count(id) != 0)
    return data[id];

  // else: die
  // TODO: don't die
  return dummy;
}

template <typename T> T &ComponentMap<T>::operator[](uuid_t id) { return get(id); }

template <typename T> bool ComponentMap<T>::erase(uuid_t id) {
  if (data.count(id) != 0) {
    data.erase(id);
    return true;
  }

  // fail
  return false;
}

template <typename T> bool ComponentMap<T>::has(uuid_t id) { return data.count(id) != 0; }