# CMake file for unit test executable

# List unit test source files
add_executable(unittests
    ../source/background.c
    ../source/cgram.c
    background_tests.cpp
    cgram_tests.cpp
    palette_tests.cpp
    snes_objects_tests.cpp
    utility_tests.cpp
)

# Add dependent libraries
target_link_libraries(unittests
    gtest
    gtest_main
    pthread
)
