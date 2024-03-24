target_include_directories(${PROJECT_NAME} PUBLIC "include")

if (WIN32)
    if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
#        add_subdirectory(win32)
        target_link_directories(${PROJECT_NAME} win32)
    else ()
#        add_subdirectory(win64)
        target_link_directories(${PROJECT_NAME} INTERFACE win64)
    endif()
elseif(UNIX)
    if (APPLE)
        # Mac
    else()
        # Linux (Probably)
    endif ()
endif ()

