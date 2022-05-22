cmake_minimum_required(VERSION 3.16)

add_executable(pdcip_test tree_test.c)
# pthread library must be linked when using static Check target
target_link_libraries(pdcip_test pdcip_c Check::check pthread)

add_test(NAME pdcip_test COMMAND pdcip_test)
