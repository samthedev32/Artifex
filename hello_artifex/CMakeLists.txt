project(hello_artifex)

add_executable(hello_artifex main.cpp)
target_link_libraries(hello_artifex Artifex)

if (EMSCRIPTEN)
    target_link_options(${PROJECT_NAME} PRIVATE "-s USE_WEBGL2=1 -s USE_GLFW=3 -s WASM=1")
endif (EMSCRIPTEN)
