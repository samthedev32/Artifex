cmake_minimum_required(VERSION 3.8)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

project(Artifex)

# Source Files
set(SOURCES
        src/Engine.cpp

        # Asset Manager
        src/types/uuid.cpp
        src/AssetManager.cpp

        # ECS
        src/ECS/World.cpp
        src/ECS/Type.cpp
        src/ECS/Column.cpp

        # Graphics
        src/Window.cpp
        src/Renderer.cpp

        # Audio
        src/Mixer.cpp

        src/Log.cpp
)

# Libraries
set(LIBRARIES
        m

        glad

        stb
)

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

add_subdirectory(external)
add_subdirectory(examples)
