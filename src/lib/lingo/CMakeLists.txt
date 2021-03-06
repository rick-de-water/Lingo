# Create library
add_library(lingo INTERFACE)
target_compile_features(lingo INTERFACE cxx_std_11)
if (MSVC)
	target_compile_options(lingo INTERFACE "/utf-8")
endif()

# Add dependencies
target_include_directories(lingo INTERFACE  
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/..>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/..>"
	"$<INSTALL_INTERFACE:include>"
)

add_dependencies(lingo lingo_gen)

# Add sources
set(LINGO_MANUAL_HEADERS)

list(APPEND LINGO_MANUAL_HEADERS "strlen.hpp")

# Natvis
if (MSVC)
	list(APPEND LINGO_MANUAL_HEADERS "lingo.natvis")
endif()

# Platform
list(APPEND LINGO_MANUAL_HEADERS "platform/architecture.hpp")
list(APPEND LINGO_MANUAL_HEADERS "platform/attributes.hpp")
list(APPEND LINGO_MANUAL_HEADERS "platform/compiler.hpp")
list(APPEND LINGO_MANUAL_HEADERS "platform/constexpr.hpp")
list(APPEND LINGO_MANUAL_HEADERS "platform/endian.hpp")
list(APPEND LINGO_MANUAL_HEADERS "platform/pragma.hpp")
list(APPEND LINGO_MANUAL_HEADERS "platform/preprocessor.hpp")
list(APPEND LINGO_MANUAL_HEADERS "platform/warnings.hpp")
list(APPEND LINGO_MANUAL_HEADERS "platform/wchar.hpp")

# Encoding
list(APPEND LINGO_MANUAL_HEADERS "encoding/base.hpp")
list(APPEND LINGO_MANUAL_HEADERS "encoding/join.hpp")
list(APPEND LINGO_MANUAL_HEADERS "encoding/none.hpp")
list(APPEND LINGO_MANUAL_HEADERS "encoding/utf8.hpp")
list(APPEND LINGO_MANUAL_HEADERS "encoding/utf16.hpp")
list(APPEND LINGO_MANUAL_HEADERS "encoding/utf32.hpp")

list(APPEND LINGO_MANUAL_HEADERS "encoding/endian.hpp")
list(APPEND LINGO_MANUAL_HEADERS "encoding/execution.hpp")
list(APPEND LINGO_MANUAL_HEADERS "encoding/point_iterator.hpp")
list(APPEND LINGO_MANUAL_HEADERS "encoding/result.hpp")

list(APPEND LINGO_MANUAL_HEADERS "encoding/internal/bit_converter.hpp")

# Code pages
list(APPEND LINGO_MANUAL_HEADERS "page/ascii.hpp")
list(APPEND LINGO_MANUAL_HEADERS "page/iso_8859.hpp")

list(APPEND LINGO_MANUAL_HEADERS "page/unicode.hpp")
list(APPEND LINGO_MANUAL_HEADERS "page/unicode_general_catagory.hpp")
list(APPEND LINGO_MANUAL_HEADERS "page/unicode_version.hpp")

list(APPEND LINGO_MANUAL_HEADERS "page/execution.hpp")
list(APPEND LINGO_MANUAL_HEADERS "page/intermediate.hpp")
list(APPEND LINGO_MANUAL_HEADERS "page/point_mapper.hpp")
list(APPEND LINGO_MANUAL_HEADERS "page/result.hpp")

# Error handlers
list(APPEND LINGO_MANUAL_HEADERS "error/error_code.hpp")
list(APPEND LINGO_MANUAL_HEADERS "error/exception.hpp")

list(APPEND LINGO_MANUAL_HEADERS "error/replace.hpp")
list(APPEND LINGO_MANUAL_HEADERS "error/skip.hpp")
list(APPEND LINGO_MANUAL_HEADERS "error/strict.hpp")

# Utility
list(APPEND LINGO_MANUAL_HEADERS "utility/compressed_pair.hpp")
list(APPEND LINGO_MANUAL_HEADERS "utility/integer_sequence.hpp")
list(APPEND LINGO_MANUAL_HEADERS "utility/item_traits.hpp")
list(APPEND LINGO_MANUAL_HEADERS "utility/pointer_iterator.hpp")
list(APPEND LINGO_MANUAL_HEADERS "utility/span.hpp")
list(APPEND LINGO_MANUAL_HEADERS "utility/type_traits.hpp")

# Strings
list(APPEND LINGO_MANUAL_HEADERS "null_terminated_string.hpp")
list(APPEND LINGO_MANUAL_HEADERS "string.hpp" "string_storage.hpp")
list(APPEND LINGO_MANUAL_HEADERS "string_view.hpp" "string_view_storage.hpp")
list(APPEND LINGO_MANUAL_HEADERS "string_converter.hpp" "conversion_result.hpp")

# Get the generated headers
get_target_property(LINGO_GENERATED_HEADERS lingo_gen LINGO_GENERATED_HEADERS)
foreach(LINGO_GENERATED_HEADER ${LINGO_GENERATED_HEADERS})
	set_property(SOURCE "${LINGO_GENERATED_HEADER}" PROPERTY GENERATED 1)
endforeach()

set(LINGO_HEADERS ${LINGO_MANUAL_HEADERS} ${LINGO_GENERATED_HEADERS})

# Create IDE project
add_custom_target(lingo_ide SOURCES ${LINGO_HEADERS})
set_target_properties(lingo_ide PROPERTIES FOLDER Libraries)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${LINGO_MANUAL_HEADERS})
source_group(TREE "${CMAKE_CURRENT_BINARY_DIR}" FILES ${LINGO_GENERATED_HEADERS})

# Install
foreach(LINGO_HEADER ${LINGO_HEADERS})
	get_filename_component(LINGO_INSTALL_DIRECTORY "${LINGO_HEADER}" REALPATH)
	string(REPLACE "${CMAKE_CURRENT_BINARY_DIR}/" "" LINGO_INSTALL_DIRECTORY "${LINGO_INSTALL_DIRECTORY}")
	string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" LINGO_INSTALL_DIRECTORY "${LINGO_INSTALL_DIRECTORY}")
	get_filename_component(LINGO_INSTALL_DIRECTORY "${LINGO_INSTALL_DIRECTORY}" DIRECTORY)

	install(FILES ${LINGO_HEADER} DESTINATION "include/lingo/${LINGO_INSTALL_DIRECTORY}")
endforeach()

install(TARGETS lingo EXPORT lingo)