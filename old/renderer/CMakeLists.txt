add_library(axRenderer SHARED src/renderer.c src/cxx_bindings.cpp)

target_link_libraries(axRenderer PUBLIC axCommon axWindow)
target_link_libraries(axRenderer PRIVATE GL glad)

target_include_directories(axRenderer PUBLIC include)

add_subdirectory(external)