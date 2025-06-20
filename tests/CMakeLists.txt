# CMake file for unit test executable

# List unit test source files
add_executable(run_tests
    # Test filenames go here
    example_test.cpp
)

# Add dependent libraries
target_link_libraries(run_tests
    gtest
    gtest_main
    pthread
)
