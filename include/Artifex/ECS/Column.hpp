#pragma once

#include <Artifex/Log.hpp>

#include "ID.hpp"

namespace Artifex::ECS {

// Data Column
struct Column {
   public:
    explicit Column(size_t type_size, size_t initial_size = 64);
    ~Column();

    template <typename T>
    ID add();

    template <typename T>
    T& get(ID id);

    void* operator[](size_t i);

   private:
    void* elements = nullptr;  // data
    size_t size{}, count{};    // size of buffer & count of elements
    const size_t type_size;
};

template <typename T>
ID Column::add() {
    return 0;
}

template <typename T>
T& Column::get(ID id) {
    if (sizeof(T) != type_size) {
        Log::error("ECS::Column", "Wrong Type Size");
        return {};
    }
}

}  // namespace Artifex::ECS