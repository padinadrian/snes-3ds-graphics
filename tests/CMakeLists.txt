# CMake file for unit test executable

# List unit test source files
add_executable(unittests
    ${CMAKE_SOURCE_DIR}/source/cgram.c
    ${CMAKE_SOURCE_DIR}/source/tile.c
    cgram_bitmap_test.cpp
    cgram_tests.cpp
    palette_tests.cpp
    snes_objects_tests.cpp
    tile_tests.cpp
    utility_tests.cpp
)

# Include headers in tests folder
include_directories("${CMAKE_SOURCE_DIR}/tests")

# Add dependent libraries
target_link_libraries(unittests
    gtest
    gtest_main
    pthread
)
