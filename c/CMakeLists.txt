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

include_directories(include)

add_subdirectory(src)
# add_subdirectory(test)
