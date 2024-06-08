cmake_minimum_required(VERSION 3.5)

project(md5)

add_library(md5 src/md5.cpp)
target_include_directories(md5 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# MD5 implementation from
# http://www.zedwood.com/article/cpp-md5-function