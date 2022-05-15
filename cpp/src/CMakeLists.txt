cmake_minimum_required(VERSION 3.16)

# defines globs for different directories
add_subdirectory(structures)

add_library(pdcip_cpp SHARED ${STRUCTURES_CC})
