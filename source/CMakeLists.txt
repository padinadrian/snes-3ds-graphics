# CMake file for primary executable

# Generate version source file
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/version.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/version.cpp")

# Create binary executable
add_library(${PROJECT_NAME}
    background.c
    oam.c
    "${CMAKE_CURRENT_BINARY_DIR}/version.cpp"
)

# Create usage info header
set(USAGE_TXT "${CMAKE_SOURCE_DIR}/usage.txt")
set(USAGE_HPP "${CMAKE_CURRENT_BINARY_DIR}/usage.hpp")
set(USAGE_PY "${CMAKE_SOURCE_DIR}/tools/configure_usage.py")
add_custom_command(
    OUTPUT ${USAGE_HPP}
    COMMAND python ${USAGE_PY} ${USAGE_TXT} ${USAGE_HPP}
    DEPENDS ${USAGE_TXT} ${USAGE_PY}
)

# Add header as a dependency
add_custom_target(usage_header ALL DEPENDS ${USAGE_HPP})
add_dependencies(${PROJECT_NAME} usage_header)

# Add usage_header output path as an include path
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Add install command
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "/usr/local/bin")
