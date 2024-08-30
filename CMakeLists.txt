cmake_minimum_required(VERSION 3.14)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

project(Artifex)

# external libraries
add_subdirectory(external)

# main library
add_subdirectory(library)

# example programs
# add_subdirectory(examples)