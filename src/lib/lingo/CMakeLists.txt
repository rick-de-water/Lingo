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

# Add sources
set(LINGO_MANUAL_HEADERS)
set(LINGO_GENERATED_HEADERS)

list(APPEND LINGO_MANUAL_HEADERS "constexpr.hpp" "strlen.hpp")

# Natvis
if (MSVC)
	list(APPEND LINGO_MANUAL_HEADERS "lingo.natvis")
endif()

# Platform
list(APPEND LINGO_MANUAL_HEADERS "platform/wchar.hpp")

# Encoding
list(APPEND LINGO_MANUAL_HEADERS "encoding/none.hpp")
list(APPEND LINGO_MANUAL_HEADERS "encoding/utf8.hpp")
list(APPEND LINGO_MANUAL_HEADERS "encoding/utf16.hpp")

list(APPEND LINGO_MANUAL_HEADERS "encoding/cstring.hpp")
list(APPEND LINGO_MANUAL_HEADERS "encoding/point_iterator.hpp")
list(APPEND LINGO_MANUAL_HEADERS "encoding/result.hpp")

# Code pages
list(APPEND LINGO_MANUAL_HEADERS "page/ascii.hpp")
list(APPEND LINGO_MANUAL_HEADERS "page/unicode.hpp")

list(APPEND LINGO_MANUAL_HEADERS "page/iso_8859.hpp")
list(APPEND LINGO_MANUAL_HEADERS "page/internal/iso_8859.hpp")

include(unicode)
foreach(ISO_8859_PART RANGE 1 16)
	if (NOT ISO_8859_PART EQUAL 12)
		unicode_generate_headers("${CMAKE_SOURCE_DIR}/spec/unicode/MAPPINGS/ISO8859/8859-${ISO_8859_PART}.TXT" "iso_8859_${ISO_8859_PART}")
		
		list(APPEND LINGO_GENERATED_HEADERS "${CMAKE_CURRENT_BINARY_DIR}/page/internal/iso_8859_${ISO_8859_PART}_unicode_mapping.hpp")
		list(APPEND LINGO_GENERATED_HEADERS "${CMAKE_CURRENT_BINARY_DIR}/page/internal/unicode_iso_8859_${ISO_8859_PART}_mapping.hpp")
	endif()
endforeach()

list(APPEND LINGO_MANUAL_HEADERS "page/cstring.hpp")
list(APPEND LINGO_MANUAL_HEADERS "page/intermediate.hpp")
list(APPEND LINGO_MANUAL_HEADERS "page/point_mapper.hpp")
list(APPEND LINGO_MANUAL_HEADERS "page/result.hpp")

# Error handlers
list(APPEND LINGO_MANUAL_HEADERS "error/error_code.hpp")
list(APPEND LINGO_MANUAL_HEADERS "error/exception.hpp")

list(APPEND LINGO_MANUAL_HEADERS "error/replace.hpp")
list(APPEND LINGO_MANUAL_HEADERS "error/skip.hpp")
list(APPEND LINGO_MANUAL_HEADERS "error/strict.hpp")

# Strings
list(APPEND LINGO_MANUAL_HEADERS "string.hpp" "string_storage.hpp")
list(APPEND LINGO_MANUAL_HEADERS "string_view.hpp" "string_view_storage.hpp")

# Utility
list(APPEND LINGO_MANUAL_HEADERS "compressed_pair.hpp")


set(LINGO_HEADERS ${LINGO_MANUAL_HEADERS} ${LINGO_GENERATED_HEADERS})

# Create IDE project
add_custom_target(lingo_ide SOURCES ${LINGO_HEADERS})
set_target_properties(lingo_ide PROPERTIES FOLDER Libraries)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${LINGO_MANUAL_HEADERS})
source_group(TREE "${CMAKE_CURRENT_BINARY_DIR}" FILES ${LINGO_GENERATED_HEADERS})

# Install
foreach(LINGO_HEADER ${LINGO_HEADERS})
	set(LINGO_INSTALL_DIRECTORY "${LINGO_HEADER}")
	string(REPLACE "${CMAKE_CURRENT_BINARY_DIR}/" "" LINGO_INSTALL_DIRECTORY "${LINGO_HEADER}")
	string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" LINGO_INSTALL_DIRECTORY "${LINGO_INSTALL_DIRECTORY}")
	get_filename_component(LINGO_INSTALL_DIRECTORY "${LINGO_INSTALL_DIRECTORY}" DIRECTORY)

	install(FILES ${LINGO_HEADER} DESTINATION "include/lingo/${LINGO_INSTALL_DIRECTORY}")
endforeach()

install(TARGETS lingo EXPORT lingo)
install(EXPORT lingo DESTINATION .)