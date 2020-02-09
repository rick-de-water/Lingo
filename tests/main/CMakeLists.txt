# Create library
add_library(test_main STATIC)
set_target_properties(test_main PROPERTIES FOLDER Tests)

target_compile_features(test_main PUBLIC "cxx_std_${LINGO_TEST_CXX_VERSION}")

if (CMAKE_CXX_COMPILER_ID MATCHES MSVC)
	target_compile_options(test_main PUBLIC /W4 /MP /bigobj)
	if (LINGO_WARNINGS_AS_ERRORS)
		target_compile_options(test_main PUBLIC /WX)
	endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES GCC)
	target_compile_options(test_main PUBLIC -Wall -Wextra -pedantic
		-Wno-unused-local-typedefs)
	if (LINGO_WARNINGS_AS_ERRORS)
		target_compile_options(test_main PUBLIC -Werror)
	endif()
	if (LINGO_CONVERAGE)
		target_compile_options(test_main PUBLIC -fprofile-arcs -ftest-coverage)
	endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES Clang)
	target_compile_options(test_main PUBLIC -Wall -Wextra -pedantic
		-Wno-unknown-warning-option
		-Wno-unused-local-typedefs
		-Wno-self-assign-overloaded)
	if (LINGO_WARNINGS_AS_ERRORS)
		target_compile_options(test_main PUBLIC -Werror)
	endif()
	if (LINGO_SANITIZE)
		target_compile_options(test_main PUBLIC -fsanitize=address -fno-omit-frame-pointer)
		target_link_options(test_main PUBLIC -fsanitize=address)
	endif()
endif()

# Add dependencies
target_link_libraries(test_main catch)

# Add sources
target_sources(test_main PRIVATE "main.cpp")