# host_test/test_display_gfx/CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

# Common CMake configuration for all tests in the host_test directory.
include("../extra_dirs_common.cmake")

# Define the project name for this test application.
project(test_display_gfx)

# Set the patterns to extract from the binary
set(EXTRACT_PATTERNS_test_display_gfx "*/display_gfx/src/*.cpp" "*/display_gfx/include/*.hpp")
set(REMOVE_PATTERNS_test_display_gfx "*/display_gfx/include/interfaces/*.hpp")

# Include common coverage logic
include("../coverage_common.cmake")
setup_gtest_coverage(test_display_gfx test_display_gfx.elf)
