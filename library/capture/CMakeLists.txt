add_library(axCapture SHARED src/capture.c src/cxx_bindings.cpp)

target_link_libraries(axCapture PUBLIC axCommon)
target_link_libraries(axCapture PRIVATE openal)

target_include_directories(axCapture PUBLIC include)