cmake_minimum_required(VERSION 3.16)

# build object targets to link into shared library
add_subdirectory(structures)

add_library(pdcip_cpp SHARED $<TARGET_OBJECTS:pdcip_cpp_structures>)
