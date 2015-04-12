cmake_minimum_required(VERSION 2.8)
project(gcc_dojo)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

add_subdirectory(lib_cspec)
add_subdirectory(src)
add_subdirectory(test)
