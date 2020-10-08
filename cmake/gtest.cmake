macro(install_gtest)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY      https://github.com/google/googletest.git
        GIT_TAG             release-1.10.0
    )

    FetchContent_GetProperties(googletest)
    if(NOT googletest_POPULATED)

        FetchContent_Populate(googletest)
        set(GOOGLETEST_FORCE_SHARED_CRT ON CACHE BOOL "Use shared crt for gtest instead static")

        if(GOOGLETEST_FORCE_SHARED_CRT)
            set(gtest_force_shared_crt ON CACHE BOOL "")
        else()
            set(gtest_force_shared_crt OFF CACHE BOOL "")
        endif()

        set(INSTALL_GTEST OFF CACHE BOOL "")
        mark_as_advanced(INSTALL_GTEST)

        add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
        
        set_target_properties(gtest PROPERTIES FOLDER extern)
        set_target_properties(gtest_main PROPERTIES FOLDER extern)
        set_target_properties(gmock PROPERTIES FOLDER extern)
        set_target_properties(gmock_main PROPERTIES FOLDER extern)
    endif()
endmacro(install_gtest)

macro(add_gtest)
    set(Optional )
    set(OneValue NAME)
    set(MultiValue SOURCES LIBRARIES)
    cmake_parse_arguments(
        ADD_GTEST
        "${Optional}"
        "${OneValue}"
        "${MultiValue}"
        ${ARGN})

    message(STATUS "Adding test: Name is ${ADD_GTEST_NAME}; Sources are ${ADD_GTEST_SOURCES}; Libs are ${ADD_GTEST_LIBRARIES}")
    add_executable(${ADD_GTEST_NAME} ${ADD_GTEST_SOURCES})
    target_link_libraries(${ADD_GTEST_NAME} PRIVATE gtest gtest_main gmock ${ADD_GTEST_LIBRARIES})

    gtest_discover_tests(${ADD_GTEST_NAME})
endmacro(add_gtest)

