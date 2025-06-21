# CMake file for unit test executable

# List unit test source files
add_executable(run_tests
    ../source/background.c
    background_tests.cpp
    snes_objects_tests.cpp
)

# Add dependent libraries
target_link_libraries(run_tests
    gtest
    gtest_main
    pthread
)
