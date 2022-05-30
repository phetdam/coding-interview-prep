cmake_minimum_required(VERSION 3.16)

include(GoogleTest)

add_executable(pdcip_test tree_test.cc)
target_link_libraries(pdcip_test pdcip_c GTest::Main)

gtest_discover_tests(pdcip_test)

# build legacy tests that use the Check C unit testing framework
if(BUILD_CHECK_TESTS)
    # soft dependency on the Check C unit testing framework
    set(CHECK_MIN_VERSION 0.15)

    # Check has its own CMake targets and package, so we can use find_package.
    # use CHECK_ROOT env variable if you need to manually specify install path
    find_package(Check ${CHECK_MIN_VERSION} PATHS $ENV{CHECK_ROOT})

    # only build tests if compatible Check library is found
    if(Check_FOUND AND Check_VERSION VERSION_GREATER_EQUAL ${CHECK_MIN_VERSION})
        add_executable(pdcip_legacy_test tree_test.c)
        target_link_libraries(pdcip_legacy_test pdcip_c Check::checkShared)
        add_test(NAME pdcip_legacy_test COMMAND pdcip_legacy_test)
    else()
        message(
            STATUS
            "Check >=${CHECK_MIN_VERSION} not satisfied. \
No legacy tests to build"
        )
    endif()
endif()
