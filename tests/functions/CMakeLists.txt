# Create executable
add_executable(test_functions strlen.cpp)
set_target_properties(test_functions PROPERTIES FOLDER Tests)

# Add dependencies
target_link_libraries(test_functions test_main)

# Discover tests
catch_discover_tests(test_functions)