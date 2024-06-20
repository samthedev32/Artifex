add_library(Artifex SHARED src/engine.c src/cxx_bindings.cpp)

# target_link_libraries(Artifex PRIVATE )

target_include_directories(Artifex PUBLIC include)