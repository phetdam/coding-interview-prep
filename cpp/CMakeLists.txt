cmake_minimum_required(VERSION 3.16)

project(
    pdcip-cpp
    VERSION 0.1.0
    DESCRIPTION "C++ interview code"
    HOMEPAGE_URL https://github.com/phetdam/coding-interview-prep/tree/master/cpp
    LANGUAGES CXX
)

# build debug by default; we set -DCMAKE_BUILD_TYPE=Release for release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# in lieu of specifying -fPIC manually to g++
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# export all symbols so we can dispense with __declspec pain (mostly)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# debug and release flags, handle MSVC case specifically
if(MSVC)
    add_compile_options(
        /Wall
        /wd4265
        /wd4514
        /wd4619
        /wd4668
        /wd4820
        /wd5045
        /wd5204
    )
    if(CMAKE_BUILD_TYPE STREQUAL Release)
        add_compile_options(/O2)
    else()
        add_compile_options(/Od /DEBUG)
    endif()
else()
    add_compile_options(-Wall -Wextra)
    if(CMAKE_BUILD_TYPE STREQUAL Release)
        add_compile_options(-O3)
    else()
        add_compile_options(-O0 -ggdb)
    endif()
endif()

# soft dependency on Google Test
set(GTEST_MIN_VERSION 1.10.0)

# use GTEST_ROOT env variable if you need to manually specify install path
find_package(GTest ${GTEST_MIN_VERSION})

include_directories(include)

add_subdirectory(src)

# only build tests and enable testing if compatible GoogleTest is found.
# note: on Windows, GTest_VERSION doesn't seem to be locatable, so there is
# a bit of hack here to ignore the version on Windows
if(
    GTest_FOUND AND
    (WIN32 OR GTest_VERSION VERSION_GREATER_EQUAL ${GTEST_MIN_VERSION})
)
    enable_testing()
    add_subdirectory(test)
else()
    message(
        STATUS
        "GoogleTest >=${GTEST_MIN_VERSION} not satisfied. No tests to build"
    )
endif()
