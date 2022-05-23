#!/usr/bin/env bash
#
# CMake wrapper script to build CMake targets in a specified subdirectory.
# Any arguments before --build-args are passed directly to cmake --build.

# arguments passed to cmake command directly
CMAKE_ARGS=()
# arguments passed to cmake --build command directly
CMAKE_BUILD_ARGS=()
# usage printed when given -h,--help
USAGE="usage: $0 src_dir [-h] [args ...] [--build-args build_args ...]

CMake wrapper script to build CMake targets given a specified subdirectory.

The specified subdirectory corresponds to the CMAKE_SOURCE_DIR CMake variable.
The corresponding build directory will be src_dir/build.

positional arguments:
 src_dir  Top-level source directory with CMakeLists.txt

optional arguments:
 -h, --help      show this usage
 args ...        extra arguments to pass to the cmake command
 build_args ...  extra arguments to pass to the cmake --build command"

##
# Collect incoming arguments, separating them for cmake, cmake --build.
#
# Sets the CMAKE_ARGS and CMAKE_BUILD_ARGS global variables.
#
# Arguments:
#   Array of command-line arguments
#       Any arguments after --build-args are passed to cmake --build while the
#       rest preceding will be passed to the cmake configure command.
#
collect_args() {
    # set to 1 after we see the --build-args flag
    POPULATE_BUILD_ARGS=0
    for ARG in "$@"
    do
        if [ "$ARG" = --build-args ]
        then
            POPULATE_BUILD_ARGS=1
        elif [ $POPULATE_BUILD_ARGS -eq 1 ]
        then
            CMAKE_BUILD_ARGS+=("$ARG")
        else
            CMAKE_ARGS+=("$ARG")
        fi
    done
}

##
# Main function for the build script.
#
main() {
    # save first arg as directory we want to run cmake on and drop args. use
    # absolute path since we will be calling this script from other dirs.
    CMAKE_SOURCE_DIR=$(realpath $1)
    shift
    if [ $CMAKE_SOURCE_DIR == -h ] || [ $CMAKE_SOURCE_DIR == --help ]
    then
        echo $USAGE
        return 0
    fi
    # separate incoming args into those for cmake, cmake --build
    collect_args "$@"
    cmake -S $CMAKE_SOURCE_DIR -B $CMAKE_SOURCE_DIR/build ${CMAKE_ARGS[@]}
    cmake --build $CMAKE_SOURCE_DIR/build ${CMAKE_BUILD_ARGS[@]}
}

set -e
main "$@"
