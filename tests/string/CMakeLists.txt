# Create executable
add_executable(test_string sso.cpp)
set_target_properties(test_string PROPERTIES FOLDER Tests)

# Add dependencies
target_link_libraries(test_string test_main)

# Discover tests
catch_discover_tests(test_string)