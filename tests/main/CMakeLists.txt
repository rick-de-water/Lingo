# Create library
add_library(test_main STATIC main.cpp)

# Add dependencies
target_link_libraries(test_main catch)