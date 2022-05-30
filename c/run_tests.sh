#!/usr/bin/env bash
#
# Run all Google Test C tests registered with CMake. If -DBUILD_CHECK_TESTS=1
# was specified during the build, then Check C tests are also run.

##
# Main function.
#
# Arguments:
#   List of command-line arguments
#
main() {
    # GTEST_COLOR=yes allows color output from Google test mains
    # output suppressed by default, so set CK_VERBOSITY=verbose in order to
    # eliminate need to specify again when we rerun with --verbose flag
    GTEST_COLOR=yes CK_VERBOSITY=verbose ctest --test-dir build "$@"
}

set -e
main "$@"
