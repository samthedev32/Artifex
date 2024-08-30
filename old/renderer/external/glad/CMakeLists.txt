add_library(glad src/glad.c)

target_link_libraries(glad PRIVATE GL)

target_include_directories(glad PUBLIC include)