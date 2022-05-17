cmake_minimum_required(VERSION 3.16)

project(
    pdcip-c
    VERSION 0.1.0
    DESCRIPTION "C interview code"
    HOMEPAGE_URL https://github.com/phetdam/coding-interview-prep/tree/master/c
    LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(include)

add_subdirectory(src)
# add_subdirectory(test)
