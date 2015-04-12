cmake_minimum_required(VERSION 2.8)

project(gcc_dojo_core)

add_library( memoryAlloc
    memoryAlloc.h memoryAlloc.c )

target_link_libraries(
    memoryAlloc
)

