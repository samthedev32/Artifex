cmake_minimum_required(VERSION 3.8)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Dependency Libs
add_subdirectory(mathutil)

# Source Files
set(SOURCES
    src/Engine.cpp

    # Core
    src/core/Window.cpp
    src/core/Render.cpp
    src/core/Load.cpp
    src/core/UI.cpp

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
if(EMSCRIPTEN)
    target_link_options(${PROJECT_NAME} PRIVATE "-s USE_WEBGL2=1 -s USE_GLFW=3 -s WASM=1")
endif(EMSCRIPTEN)
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC "include")

add_subdirectory(hello_artifex)
