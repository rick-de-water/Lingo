add_library(catch INTERFACE)

target_sources(catch INTERFACE catch.hpp)
target_include_directories(catch INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/..")