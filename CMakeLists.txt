cmake_minimum_required(VERSION 3.13)
set(CMAKE_USER_MAKE_RULES_OVERRIDE "${CMAKE_CURRENT_LIST_DIR}/cmake/CompilerOptions.cmake")

# Create project
project(lingo)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Add module paths
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Include modules
include(targets)

# Enable testing
enable_testing()

# Add subdirectories
add_subdirectory(src)
add_subdirectory(tests)