cmake_minimum_required(VERSION 3.8)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Project
project(Artifex LANGUAGES C CXX)

# Source Files
set(SOURCES
        src/Artifex/Engine.cpp

        ## Graphics
        src/Artifex/graphics/Window.cpp

        src/Artifex/graphics/renderer.cpp
        src/Artifex/graphics/load.cpp
        src/Artifex/graphics/draw.cpp

        src/Artifex/graphics/shader.cpp

        src/include/GL/glad.c

        ## Audio
        src/Artifex/audio/Mix.cpp
        src/Artifex/audio/Capture.cpp

        # Physics
        src/Artifex/physics/physics.cpp

        # Utility
        src/Artifex/utility/Log.cpp
        src/Artifex/utility/Clock.cpp
        src/Artifex/utility/uuid.c
        src/Artifex/modules/Graphics.cpp
)

# Libraries
set(LIBRARIES m)

if (WIN32)
    # Windows Libraries
    list(APPEND LIBRARIES opengl32 glfw3 soft_oal)
else ()
    # Linux & other
    list(APPEND LIBRARIES GL glfw openal)
endif ()

add_library(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC "include")
target_include_directories(${PROJECT_NAME} PRIVATE "src/include")

add_subdirectory(external)

add_subdirectory(examples)
