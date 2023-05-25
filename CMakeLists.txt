cmake_minimum_required(VERSION 3.8)

set(CMAKE_PREFIX_PATH "/usr/lib;/usr/local/lib")

find_package(glfw3 REQUIRED)

project(Artifex)

add_library(Artifex
    src/Artifex.cpp

    src/Window.cpp
    )

target_link_libraries(Artifex SHARED glfw)
target_include_directories(Artifex PUBLIC include)

add_subdirectory(hello_world)