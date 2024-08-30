add_library(axWindow SHARED src/window.c src/cxx_bindings.cpp)

target_link_libraries(axWindow PUBLIC axCommon)
target_link_libraries(axWindow PRIVATE glfw)

target_include_directories(axWindow PUBLIC include)