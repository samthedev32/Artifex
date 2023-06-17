cmake_minimum_required(VERSION 3.8)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Source Files
set(SOURCES
    src/Artifex.cpp
    src/Window.cpp

    src/GL/glad.c
    src/GL/stb_image.c
)

# Libraries
set(LIBRARIES
    glfw
)

# Project
project(Artifex VERSION 0.1.0 LANGUAGES C CXX)

add_library(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC "include")


add_subdirectory(hello_artifex)
