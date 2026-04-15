# CMake file for unit test executable

# List unit test source files
add_executable(tileset_bitmap
    ${CMAKE_SOURCE_DIR}/source/background.c
    tileset_bitmap.cpp
)

# Include headers in tests folder
include_directories("${CMAKE_SOURCE_DIR}/tests")
include_directories("${CMAKE_SOURCE_DIR}/ext")
