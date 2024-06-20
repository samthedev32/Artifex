add_library(axCamera SHARED src/camera.c src/cxx_bindings.cpp)

target_link_libraries(axCamera PUBLIC axCommon)
target_link_libraries(axCamera PRIVATE uvc)

target_include_directories(axCamera PUBLIC include)