set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(SOURCES
    src/engine.c
    
    # core types
    src/core/types/color3.c
    src/core/types/color4.c
    src/core/types/vec2.c
    src/core/types/vec3.c

    # core utils
    src/core/utils/clock.c
    src/core/utils/log.c

    # ecs
    src/ecs/typeless.c
    src/ecs/freelist.c
    )

add_library(Artifex ${SOURCES})

target_link_libraries(Artifex PRIVATE m)

target_include_directories(Artifex PUBLIC include)
target_include_directories(Artifex PRIVATE src/include)
