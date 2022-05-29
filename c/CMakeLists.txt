cmake_minimum_required(VERSION 3.16)

project(
    pdcip-c
    VERSION 0.1.0
    DESCRIPTION "C interview code"
    HOMEPAGE_URL https://github.com/phetdam/coding-interview-prep/tree/master/c
    LANGUAGES C
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
