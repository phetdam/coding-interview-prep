#!/usr/bin/env bash
#
# Run all Google Test C++ tests registered with CMake.

##
# Main function.
#
# Arguments:
#   List of command-line arguments
#
main() {
    # GTEST_COLOR=yes allows color output from Google test mains
    GTEST_COLOR=yes ctest --test-dir build "$@"
}

set -e
main "$@"
