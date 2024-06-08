cmake_minimum_required(VERSION 3.5)

project(stb)

add_library(stb src/stb_image.c src/stb_vorbis.c)
target_include_directories(stb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)