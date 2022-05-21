cmake_minimum_required(VERSION 3.16)

project(
    pdcip-c
    VERSION 0.1.0
    DESCRIPTION "C interview code"
    HOMEPAGE_URL https://github.com/phetdam/coding-interview-prep/tree/master/c
    LANGUAGES C
)

# build debug by default; we set -DCMAKE_BUILD_TYPE=Release for release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
# in lieu of specifying -fPIC manually to g++
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# export all symbols so we can dispense with __declspec pain (mostly)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# debug and release flags, handle MSVC case specifically
if(MSVC)
    add_compile_options(/Wall /wd5045)
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

# soft dependency on the Check C unit testing framework
set(CHECK_MIN_VERSION 0.15)

# Check defines its own CMake targets and package, so we can use find_package.
# use CHECK_ROOT env variable if you need to manually specify install path
find_package(Check ${CHECK_MIN_VERSION} PATHS $ENV{CHECK_ROOT})

include_directories(include)

add_subdirectory(src)

# only build tests and enable testing if compatible Check library is found
if(Check_FOUND AND Check_VERSION VERSION_GREATER_EQUAL ${CHECK_MIN_VERSION})
    enable_testing()
    add_subdirectory(test)
else()
    message(
        STATUS
        "Check >=${CHECK_MIN_VERSION} not satisfied. No tests to build"
    )
endif()
