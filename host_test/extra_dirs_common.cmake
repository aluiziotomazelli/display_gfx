# host_test/extra_dirs_common.cmake
# Common setup for host-based test projects.

# The component root is one level up from host_test directory
get_filename_component(COMPONENT_ROOT "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)

# Append extra component directories so IDF can find our component and test dependencies
list(APPEND EXTRA_COMPONENT_DIRS
    "${COMPONENT_ROOT}"
    "${CMAKE_CURRENT_LIST_DIR}/gtest"
    "${CMAKE_CURRENT_LIST_DIR}/common"
)

# Explicitly list the components to be included in the build
set(COMPONENTS 
    "main"
    "display_gfx"
    "gtest"
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)