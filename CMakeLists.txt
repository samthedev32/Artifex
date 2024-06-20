cmake_minimum_required(VERSION 3.8)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

project(Artifex)

# # Source Files
# set(SOURCES
#         src/Engine.cpp

#         # Asset Manager
#         src/types/uuid.cpp
#         src/AssetManager.cpp

#         # ECS
#         src/ECS/World.cpp
#         src/ECS/Type.cpp
#         src/ECS/Column.cpp

#         # Graphics
#         src/Window.cpp
#         src/Renderer.cpp

#         # Audio
#         src/Mixer.cpp

#         src/Log.cpp
# )

# # Libraries
# set(LIBRARIES
#         glad
#         stb
# )

# if (WIN32)
#     # Windows Libraries
#     list(APPEND LIBRARIES glfw3 OpenAL32)
# else ()
#     # Linux & other
#     list(APPEND LIBRARIES glfw openal)
# endif ()

# add_library(${PROJECT_NAME} ${SOURCES})

add_subdirectory(external)

# target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBRARIES})
# target_include_directories(${PROJECT_NAME} PUBLIC "include")

add_subdirectory(examples)
add_subdirectory(library)
