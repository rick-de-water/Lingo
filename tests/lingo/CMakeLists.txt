# Create executable
lingo_add_executable(test_lingo)
lingo_target_folder(test_lingo Tests)

# Add dependencies
target_link_libraries(test_functions test_main lingo)

# Add sources


# Discover tests
catch_discover_tests(test_functions)