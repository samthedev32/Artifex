set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(glad STATIC src/glad.c)

target_include_directories(glad PUBLIC include)