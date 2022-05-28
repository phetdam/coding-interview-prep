#!/usr/bin/env bash
#
# Run all Check C test runners registered with CMake.

##
# Main function.
#
# Arguments:
#   List of command-line arguments
#
main() {
    # output suppressed by default, so set CK_VERBOSITY=verbose in order to
    # eliminate need to specify again when we rerun with --verbose flag
    CK_VERBOSITY=verbose ctest --test-dir build "$@"
}

set -e
main "$@"
