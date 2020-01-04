# Create library
add_library(test_main STATIC)
set_target_properties(test_main PROPERTIES FOLDER Tests)

target_compile_features(test_main PUBLIC "cxx_std_${LINGO_TEST_CXX_VERSION}")

if (MSVC)
	target_compile_options(test_main PUBLIC "/W4" "/MP" "/bigobj")
else()
	target_compile_options(test_main PUBLIC -Wall -Wextra -pedantic -Wno-unknown-warning-option -Wno-unused-local-typedefs -Wno-self-assign-overloaded)
endif()

# Add dependencies
target_link_libraries(test_main catch)

# Add sources
target_sources(test_main PRIVATE "main.cpp")