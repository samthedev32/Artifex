# basic renderer
add_library(axRenderer src/renderer.c)
target_link_libraries(axRenderer PRIVATE Artifex)
target_include_directories(axRenderer PUBLIC include)
target_include_directories(axRenderer PRIVATE src/include)