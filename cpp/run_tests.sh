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
    # GTEST_COLOR=yes allows color output from Google test mains. the nested
    # command substitution gets half the number of available threads on system.
    # note that -j x can be overridden with a subsequent -j y.
    GTEST_COLOR=yes ctest --test-dir build -j $(expr $(nproc --all) / 2) "$@"
}

set -e
main "$@"
