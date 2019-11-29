# Create test
add_executable(test_string_view string_view.cpp)

# Add dependencies
target_link_libraries(test_string_view test_main lingo)

# Discover tests
catch_discover_tests(test_string_view)