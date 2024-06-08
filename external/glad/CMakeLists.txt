cmake_minimum_required(VERSION 3.5)

project(glad)

add_library(glad src/glad.c)

if (WIN32)
    target_link_libraries(glad opengl32)
else()
    target_link_libraries(glad GL)
endif()

target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
