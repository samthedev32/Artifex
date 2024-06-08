add_subdirectory(glad)
add_subdirectory(stb)

# add_subdirectory(md5) # unused at the moment

if (WIN32)
    add_subdirectory(windows)
endif()