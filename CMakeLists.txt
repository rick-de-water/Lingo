cmake_minimum_required(VERSION 3.12)

# Create project
project(lingo)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set install prefix
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH "Installation path" FORCE)
endif()

# Set options
set(LINGO_SPEC_DIRECTORY "${CMAKE_SOURCE_DIR}/spec" CACHE PATH "The location of all spec files")
set(LINGO_TEST_CXX_VERSION 11 CACHE STRING "The version of C++ to test with")
option(LINGO_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
option(LINGO_SANITIZE "Run santizers" OFF)
option(LINGO_COVERAGE "Use code coverage" OFF)

# Add module paths
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Enable testing
enable_testing()

# Add subdirectories
add_subdirectory(src)
add_subdirectory(tests)

# Install
install(EXPORT lingo DESTINATION .)
install(FILES "LICENSE" "README.md" DESTINATION ".")
install(DIRECTORY "doc" DESTINATION ".")