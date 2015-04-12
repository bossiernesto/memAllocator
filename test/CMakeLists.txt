message( STATUS ${CMAKE_SOURCE_DIR} )

include_directories( ${CMAKE_SOURCE_DIR}/src )
include_directories( ${CMAKE_SOURCE_DIR}/lib_cspec )

add_executable( memoryAllocTest
    memoryAllocTest.c
)

target_link_libraries( memoryAllocTest
    cspec
    memoryAlloc
)
