#include <Artifex/ECS/Column.hpp>

namespace Artifex::ECS {

Column::Column(size_t type_size, size_t initial_size)
    : type_size(type_size) {
    size = initial_size;
    count = 0;
    elements = malloc(size * type_size);
}

Column::~Column() {
    free(elements);
    size = 0;
    count = 0;
}

void* Column::operator[](size_t i) {
    if (i >= size)
        return nullptr;

    return (void*)((size_t)elements + (i * type_size));
}

}  // namespace Artifex::ECS