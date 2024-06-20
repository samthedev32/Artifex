add_library(axMixer SHARED src/mixer.c src/cxx_bindings.cpp)

target_link_libraries(axMixer PUBLIC axCommon)
target_link_libraries(axMixer PRIVATE openal)

target_include_directories(axMixer PUBLIC include)