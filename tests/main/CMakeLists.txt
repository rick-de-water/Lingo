# Create library
lingo_add_library(test_main STATIC)
lingo_target_folder(test_main Tests)

# Add dependencies
target_link_libraries(test_main catch lingo)
target_include_directories(test_main INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

# Add sources
lingo_target_sources(test_main PRIVATE "main.cpp" FOLDER "Main")
lingo_target_sources(test_main PRIVATE "test_types.hpp" "test_case.hpp" "test_strings.hpp" FOLDER "Tests Helpers")