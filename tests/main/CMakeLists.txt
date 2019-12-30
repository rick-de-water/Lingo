# Create library
add_library(test_main STATIC)
set_target_properties(test_main PROPERTIES FOLDER Tests)

if (MSVC)
	target_compile_options(test_main PUBLIC "/W4" "/MP" "/bigobj")
endif()

# Add dependencies
target_link_libraries(test_main catch)

# Add sources
target_sources(test_main PRIVATE "main.cpp")