cmake_minimum_required(VERSION 3.16)

# Include the project build tools
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Point to display_gfx component directory which is the parent of examples/
set(EXTRA_COMPONENT_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../../")

project(basic_usage)
