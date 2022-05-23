#!/usr/bin/env bash
#
# Thin wrapper for ../tools/cmake_build.sh to build the C library and tests.

WORKING_DIR=$(realpath .)

##
# Main function.
#
main() {
    $WORKING_DIR/../tools/cmake_build.sh $WORKING_DIR "$@"
}

set -e
main "$@"
