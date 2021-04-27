# Add sources
set(TEST_LINGO_MANUAL_SOURCES)
list(APPEND TEST_LINGO_MANUAL_SOURCES "test/test_types.hpp")
list(APPEND TEST_LINGO_MANUAL_SOURCES "test/test_case.hpp")
list(APPEND TEST_LINGO_MANUAL_SOURCES "test/test_strings.hpp")
list(APPEND TEST_LINGO_MANUAL_SOURCES "test/tuple_matrix.hpp")

# Platform
list(APPEND TEST_LINGO_MANUAL_SOURCES "platform/endian.cpp")

# Encoding
list(APPEND TEST_LINGO_MANUAL_SOURCES "encoding/base.cpp")
list(APPEND TEST_LINGO_MANUAL_SOURCES "encoding/endian.cpp")
list(APPEND TEST_LINGO_MANUAL_SOURCES "encoding/utf8.cpp")
list(APPEND TEST_LINGO_MANUAL_SOURCES "encoding/utf16.cpp")
list(APPEND TEST_LINGO_MANUAL_SOURCES "encoding/utf32.cpp")

# Pages
list(APPEND TEST_LINGO_MANUAL_SOURCES "page/ascii.cpp")
list(APPEND TEST_LINGO_MANUAL_SOURCES "page/iso_8859.cpp")
list(APPEND TEST_LINGO_MANUAL_SOURCES "page/unicode.cpp")

# Utility
list(APPEND TEST_LINGO_MANUAL_SOURCES "utility/pointer_iterator.cpp")

# Strings
list(APPEND TEST_LINGO_MANUAL_SOURCES "null_terminated_string.cpp")
list(APPEND TEST_LINGO_MANUAL_SOURCES "string.cpp")
list(APPEND TEST_LINGO_MANUAL_SOURCES "string_view.cpp")
list(APPEND TEST_LINGO_MANUAL_SOURCES "string_converter.cpp")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${TEST_LINGO_MANUAL_SOURCES})
source_group(TREE "${CMAKE_CURRENT_BINARY_DIR}" FILES ${TEST_LINGO_GENERATED_SOURCES})

# Create executables
function(lingo_create_test_executable TARGET_NAME TEST_PREFIX)
	# Create executable
	add_executable("${TARGET_NAME}")
	set_target_properties("${TARGET_NAME}" PROPERTIES FOLDER Tests)
	
	# Add settings
	target_compile_definitions("${TARGET_NAME}"
		PRIVATE "LINGO_SPEC_DIRECTORY=\"${LINGO_SPEC_DIRECTORY}\"" ${ARGN})

	# Add dependencies
	target_link_libraries("${TARGET_NAME}" test_main lingo)
	target_include_directories("${TARGET_NAME}"
		PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_BINARY_DIR}/..")

	# Add sources
	target_sources("${TARGET_NAME}" PRIVATE ${TEST_LINGO_MANUAL_SOURCES} ${TEST_LINGO_GENERATED_SOURCES})
		
	# Discover tests
	catch_discover_tests("${TARGET_NAME}" TEST_PREFIX "${TEST_PREFIX}")
endfunction()

# Discover tests
if (LINGO_TEST_SPLIT)
	lingo_create_test_executable(test_lingo_split "[split]" LINGO_TEST_SPLIT)
endif()

if (LINGO_TEST_SINGLE)
	lingo_create_test_executable(test_lingo_single "[single]" LINGO_TEST_SINGLE)
endif()

if (LINGO_TEST_MODULE)
	#lingo_create_test_executable(test_lingo_module "[module]" LINGO_TEST_MODULE)
endif()