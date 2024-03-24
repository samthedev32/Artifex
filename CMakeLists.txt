cmake_minimum_required(VERSION 3.8)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Project
project(Artifex VERSION 0.1.0 LANGUAGES C CXX)

# Source Files
set(SOURCES
        #        src/Engine.cpp

        # Core
        src/Artifex/utility/Log.cpp

        src/Artifex/renderer/window.cpp

        ## Renderer
        src/Artifex/renderer/renderer.cpp
        src/Artifex/renderer/load.cpp
        src/Artifex/renderer/draw.cpp

        src/Artifex/renderer/shader.cpp

        src/Artifex/renderer/GL/glad.c

        src/Artifex/core/Mix.cpp

        src/Artifex/types/module.cpp

        src/Artifex/utility/uuid.c
        src/Artifex/utility/RAW/shader.cpp
)

# Libraries
set(LIBRARIES m)

if (WIN32)
    # Windows Libraries
    list(APPEND LIBRARIES opengl32 glfw3 soft_oal)
else ()
    # Linux & other
    list(APPEND LIBRARIES GL glfw AL)
endif ()

add_library(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC "include")

add_subdirectory(external)

add_subdirectory(examples)
