set(CMAKE_POSITION_INDEPENDENT_CODE ON)


add_library(axCommon src/vec.c src/clock.c src/log.c)

target_link_libraries(axCommon PRIVATE m)

target_include_directories(axCommon PUBLIC include)