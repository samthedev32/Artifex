cmake_minimum_required(VERSION 3.8)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

find_package(glfw3 REQUIRED)
link_libraries(glfw)
include_directories("include")

project(Artifex)

add_library(Artifex
    src/Artifex.cpp

    src/Window.cpp

    src/load.cpp

    src/glad.c
    src/stb_image.c
)

add_subdirectory(hello_artifex)
