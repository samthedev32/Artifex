cmake_minimum_required(VERSION 3.5)

project(glad)

add_library(glad src/glad.c)
target_link_libraries(glad GL)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
