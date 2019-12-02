# Create library
add_library(test_main STATIC main.cpp test_strings.hpp)
set_target_properties(test_main PROPERTIES FOLDER Tests)

# Add dependencies
target_link_libraries(test_main catch lingo)
target_include_directories(test_main INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")