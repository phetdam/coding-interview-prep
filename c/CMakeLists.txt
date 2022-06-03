cmake_minimum_required(VERSION 3.16)

project(
    pdcip-c
    VERSION 0.1.0
    DESCRIPTION "C implementations of data structures and algorithms"
    HOMEPAGE_URL https://github.com/phetdam/pdcip
    LANGUAGES C CXX
)

# update CMake module path. see ../tools/cmake for all relevant modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../tools/cmake")

# set default build type if not specified and set C++ project defaults
include(SetDefaultBuild)
include(SetCppDefaults)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# debug and release flags, handle MSVC case specifically
if(MSVC)
    # Check causes C4100 warning. see definition of START_TEST macro
    add_compile_options(/Wall /wd4100 /wd5045)
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

# build C tests using Google Test if minimum version of Google Test found
include(BuildGTestTests)
