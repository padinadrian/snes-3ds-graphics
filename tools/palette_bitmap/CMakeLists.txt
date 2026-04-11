# CMake file for unit test executable

# List unit test source files
add_executable(palette_bitmap
    palette_bitmap.cpp
)

# Include headers in tests folder
include_directories("${CMAKE_SOURCE_DIR}/tests")
include_directories("${CMAKE_SOURCE_DIR}/ext")

# Add dependent libraries
target_link_libraries(palette_bitmap
    gtest
    gtest_main
    pthread
)
