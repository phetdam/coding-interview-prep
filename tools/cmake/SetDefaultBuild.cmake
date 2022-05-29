cmake_minimum_required(VERSION 3.16)

# build debug by default; we set -DCMAKE_BUILD_TYPE=Release for release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()
