cmake_minimum_required(VERSION 3.16)

# all the test executables should link against gtest_main
link_libraries(GTest::Main)

include(GoogleTest)

# build test runners for each subdirectory object linked in main library. each
# of the test runners will be discovered using gtest_discover_tests
# add_subdirectory(algorithms)
add_subdirectory(structures)
