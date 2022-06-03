cmake_minimum_required(VERSION 3.16)

include(GoogleTest)

add_executable(pdcip_cpp_test graph_test.cc tree_test.cc)
target_link_libraries(pdcip_cpp_test pdcip_cpp GTest::Main)
gtest_discover_tests(pdcip_cpp_test)
