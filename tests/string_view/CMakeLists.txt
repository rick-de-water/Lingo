# Create executable
add_executable(test_string_view string_view.cpp)
set_target_properties(test_string_view PROPERTIES FOLDER Tests)

# Add dependencies
target_link_libraries(test_string_view test_main)

# Discover tests
catch_discover_tests(test_string_view)