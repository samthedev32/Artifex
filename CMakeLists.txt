cmake_minimum_required(VERSION 3.8)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Dependency Libs
add_subdirectory(EngineToolkit)

# Source Files
set(SOURCES
    src/Engine.cpp

    # Math
    src/math/vec/vec2.cpp
    src/math/vec/vec3.cpp
    src/math/vec/vec4.cpp

    src/math/mat/mat3.cpp
    src/math/mat/mat4.cpp

    # Core
    src/core/Window.cpp
    src/core/Load.cpp
    src/core/Render.cpp
    src/core/UI.cpp
    src/core/Mix.cpp

    src/types/shader.cpp

    src/GL/glad.c
)

# Libraries
set(LIBRARIES
    SDL2
    SDL2_mixer
    EngineToolkit
)

# Project
project(Artifex VERSION 0.1.0 LANGUAGES C CXX)

add_library(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC "include")

add_subdirectory(examples)
