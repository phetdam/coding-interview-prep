cmake_minimum_required(VERSION 3.16)

# all the test executables should link against gtest_main
link_libraries(GTest::Main)

include(GoogleTest)

add_executable(pdcip_cpp_test tree_test.cc)
target_link_libraries(pdcip_cpp_test pdcip_cpp)
gtest_discover_tests(pdcip_cpp_test)
