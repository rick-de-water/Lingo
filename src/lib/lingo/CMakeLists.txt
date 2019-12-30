# Create library
lingo_add_library(lingo INTERFACE)
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
lingo_target_sources(lingo INTERFACE "constexpr.hpp" "strlen.hpp")

# Natvis
if (MSVC)
	lingo_target_sources(lingo INTERFACE "lingo.natvis" FOLDER "natvis")
endif()

# Platform
lingo_target_sources(lingo INTERFACE "platform/wchar.hpp" FOLDER "platform")

# Encoding
lingo_target_sources(lingo INTERFACE "encoding/none.hpp" FOLDER "encoding/none")
lingo_target_sources(lingo INTERFACE "encoding/utf8.hpp" FOLDER "encoding/utf")
lingo_target_sources(lingo INTERFACE "encoding/utf16.hpp" FOLDER "encoding/utf")

lingo_target_sources(lingo INTERFACE "encoding/cstring.hpp" FOLDER "encoding/cstring")
lingo_target_sources(lingo INTERFACE "encoding/point_iterator.hpp" FOLDER "encoding/iterators")
lingo_target_sources(lingo INTERFACE "encoding/result.hpp" FOLDER "encoding/result")

# Code pages
lingo_target_sources(lingo INTERFACE "page/ascii.hpp" FOLDER "page/ascii")
lingo_target_sources(lingo INTERFACE "page/unicode.hpp" FOLDER "page/unicode")

lingo_target_sources(lingo INTERFACE "page/internal/iso_8859.hpp" FOLDER "page/internal/iso_8859")

include(unicode)
foreach(ISO_8859_PART RANGE 1 16)
	if (NOT ISO_8859_PART EQUAL 12)
		unicode_generate_headers("${CMAKE_SOURCE_DIR}/spec/unicode/MAPPINGS/ISO8859/8859-${ISO_8859_PART}.TXT" "iso_8859_${ISO_8859_PART}")
		configure_file("page/iso_8859_x.hpp.in" "${CMAKE_CURRENT_BINARY_DIR}/page/iso_8859_${ISO_8859_PART}.hpp")
		
		lingo_target_sources(lingo INTERFACE "${CMAKE_CURRENT_BINARY_DIR}/page/iso_8859_${ISO_8859_PART}.hpp" FOLDER "page/iso_8859")
		lingo_target_sources(lingo INTERFACE "${CMAKE_CURRENT_BINARY_DIR}/page/internal/iso_8859_${ISO_8859_PART}_unicode_mapping.hpp" FOLDER "page/internal/iso_8859")
		lingo_target_sources(lingo INTERFACE "${CMAKE_CURRENT_BINARY_DIR}/page/internal/unicode_iso_8859_${ISO_8859_PART}_mapping.hpp" FOLDER "page/internal/iso_8859")
	endif()
endforeach()

lingo_target_sources(lingo INTERFACE "page/cstring.hpp" FOLDER "page/cstring")
lingo_target_sources(lingo INTERFACE "page/intermediate.hpp" FOLDER "page/intermediate")
lingo_target_sources(lingo INTERFACE "page/point_mapper.hpp" FOLDER "page/point_mapper")
lingo_target_sources(lingo INTERFACE "page/result.hpp" FOLDER "page/result")

# Error handlers
lingo_target_sources(lingo INTERFACE "error/error_code.hpp" FOLDER "error")
lingo_target_sources(lingo INTERFACE "error/exception.hpp" FOLDER "error")

lingo_target_sources(lingo INTERFACE "error/replace.hpp" FOLDER "error")
lingo_target_sources(lingo INTERFACE "error/skip.hpp" FOLDER "error")
lingo_target_sources(lingo INTERFACE "error/strict.hpp" FOLDER "error")

# Strings
lingo_target_sources(lingo INTERFACE "string.hpp" "string_storage.hpp" FOLDER "strings/string")
lingo_target_sources(lingo INTERFACE "string_view.hpp" "string_view_storage.hpp" FOLDER "strings/string_view")

# Utility
lingo_target_sources(lingo INTERFACE "compressed_pair.hpp" FOLDER "utility")



# Create IDE project
get_target_property(LINGO_SOURCES lingo INTERFACE_SOURCES)
add_custom_target(lingo_ide SOURCES ${LINGO_SOURCES})
lingo_target_folder(lingo_ide Libraries)

# Install
foreach(LINGO_SOURCE ${LINGO_SOURCES})
	set(LINGO_INSTALL_DIRECTORY "${LINGO_SOURCE}")
	string(REPLACE "${CMAKE_CURRENT_BINARY_DIR}/" "" LINGO_INSTALL_DIRECTORY "${LINGO_SOURCE}")
	string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" LINGO_INSTALL_DIRECTORY "${LINGO_INSTALL_DIRECTORY}")
	get_filename_component(LINGO_INSTALL_DIRECTORY "${LINGO_INSTALL_DIRECTORY}" DIRECTORY)

	install(FILES ${LINGO_SOURCE} DESTINATION "include/lingo/${LINGO_INSTALL_DIRECTORY}")
endforeach()