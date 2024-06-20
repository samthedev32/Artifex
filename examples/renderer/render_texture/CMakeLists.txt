add_executable(render_texture main.c)
target_link_libraries(render_texture axRenderer stb)

add_executable(render_texture_cxx main.cpp)
target_link_libraries(render_texture_cxx axRenderer stb)