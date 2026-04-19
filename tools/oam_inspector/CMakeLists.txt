# CMake file for unit test executable

# List unit test source files
add_executable(oam_inspector
    ${CMAKE_SOURCE_DIR}/source/oam.c
    oam_inspector.cpp
)

# Include headers in tests folder
include_directories("${CMAKE_SOURCE_DIR}/tests")
include_directories("${CMAKE_SOURCE_DIR}/ext")
