cmake_minimum_required(VERSION 3.16)

# defines globs for different directories
file(GLOB_RECURSE STRUCTURES_CC structures/*.cc)

add_library(pdcip_cpp SHARED ${STRUCTURES_CC})
