project(hello_artifex)

add_executable(hello_artifex main.cpp)
target_link_libraries(hello_artifex Artifex)

if (DEFINED EMSCRIPTEN)
	set(CMAKE_EXECUTABLE_SUFFIX ".wasm")

	set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-Os -s SIDE_MODULE=1 ")
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS    "-Os -s WASM=1 -s SIDE_MODULE=1 -s STANDALONE_WASM -s USE_GLFW=3")
endif()
