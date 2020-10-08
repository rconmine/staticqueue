cmake_minimum_required(VERSION 3.15)

project(staticqueue )

set(BUILD_TESTS ON CACHE BOOL "Build tests")

add_subdirectory(src)

if(BUILD_TESTS)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/gtest.cmake)
    install_gtest()


    enable_testing()
    include(GoogleTest)

    add_subdirectory(test)
endif(BUILD_TESTS)

