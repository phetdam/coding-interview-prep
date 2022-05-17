#!/usr/bin/env bash
#
# Simple CMake wrapper script to build the CMake targets in the project.
# Any arguments before --build-args are passed directly to cmake --build.

# arguments passed to cmake command directly
CMAKE_ARGS=()
# arguments passed to cmake --build command directly
CMAKE_BUILD_ARGS=()

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
    # separate incoming args into those for cmake, cmake --build
    collect_args "$@"
    cmake -S . -B build ${CMAKE_ARGS[@]}
    cmake --build build ${CMAKE_BUILD_ARGS[@]}
}

set -e
main "$@"
