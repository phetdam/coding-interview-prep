cmake_minimum_required(VERSION 3.16)

# soft dependency on Google Test
set(GTEST_MIN_VERSION 1.10.0)

# use GTEST_ROOT env variable if you need to manually specify install path
find_package(GTest ${GTEST_MIN_VERSION})

# only build tests and enable testing if compatible GoogleTest is found.
# note: on Windows, GTest_VERSION doesn't seem to be locatable, so there is
# a bit of hack here to ignore the version on Windows
if(
    GTest_FOUND AND
    (WIN32 OR GTest_VERSION VERSION_GREATER_EQUAL ${GTEST_MIN_VERSION})
)
    enable_testing()
    add_subdirectory(test)
else()
    message(
        STATUS
        "GoogleTest >=${GTEST_MIN_VERSION} not satisfied. No tests to build"
    )
endif()
