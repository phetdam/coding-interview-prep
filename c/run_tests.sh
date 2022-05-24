#!/usr/bin/env bash
#
# Run all Check C test runners registered with CMake.

##
# Main function.
#
main() {
    ctest --test-dir build "$@"
}

set -e
main "$@"
