cmake_minimum_required(VERSION 3.16)

add_executable(pdcip_test tree_test.c)
message(STATUS "LINK_DIRECTORIES ${LINK_DIRECTORIES}")
target_link_libraries(pdcip_test pdcip_c Check::checkShared)

add_test(NAME pdcip_test COMMAND pdcip_test)
