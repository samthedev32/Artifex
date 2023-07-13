cmake_minimum_required(VERSION 3.8)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Source Files
set(SOURCES
    src/Artifex.cpp

    # Core
    src/core/Window.cpp
    src/core/Render.cpp
    src/core/Load.cpp

    src/types/shader.cpp

    src/GL/glad.c
)

# Libraries
set(LIBRARIES
    glfw
    mathutil
)

# Project
project(Artifex VERSION 0.1.0 LANGUAGES C CXX)

add_library(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC "include")

add_subdirectory(mathutil)
add_subdirectory(hello_artifex)
