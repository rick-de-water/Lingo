# Create library
lingo_add_library(lingo INTERFACE)

target_compile_features(lingo INTERFACE cxx_std_11)

# Add settings
if (MSVC)
	# constexpr if warnings are buggy and get false positives
	target_compile_options(lingo INTERFACE /wd4127 /wd4984)
endif()

# Add dependencies
target_include_directories(lingo INTERFACE  
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/..>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/..>"
	"$<INSTALL_INTERFACE:include>"
)

# Generate code from spec files
include(unicode)
foreach(INDEX RANGE 1 16)
	if (NOT INDEX EQUAL 12)
		unicode_generate_headers("${CMAKE_SOURCE_DIR}/spec/unicode/MAPPINGS/ISO8859/8859-${INDEX}.TXT" "iso_8859_${INDEX}")
	endif()
endforeach()

# Add sources
lingo_target_sources(lingo INTERFACE "constexpr.hpp" "strlen.hpp")

# Natvis
if (MSVC)
	lingo_target_sources(lingo INTERFACE "lingo.natvis" FOLDER "natvis")
endif()

# Strings
lingo_target_sources(lingo INTERFACE "string.hpp" "string_storage.hpp" FOLDER "strings/string")
lingo_target_sources(lingo INTERFACE "string_view.hpp" "string_view_storage.hpp" FOLDER "strings/string_view")

# Code pages
lingo_target_sources(lingo INTERFACE "page/ascii.hpp" FOLDER "page/ascii")
lingo_target_sources(lingo INTERFACE "page/unicode.hpp" FOLDER "page/unicode")

lingo_target_sources(lingo INTERFACE "page/internal/iso_8859.hpp" FOLDER "page/internal/iso_8859")
lingo_target_sources(lingo INTERFACE "page/iso_8859_1.hpp" "page/iso_8859_2.hpp" FOLDER "page/internal/iso_8859")

lingo_target_sources(lingo INTERFACE "page/intermediate.hpp" FOLDER "page/intermediate")
lingo_target_sources(lingo INTERFACE "page/point_mapper.hpp" FOLDER "page/point_mapper")
lingo_target_sources(lingo INTERFACE "page/result.hpp" FOLDER "page/result")

# Encoding
lingo_target_sources(lingo INTERFACE "encoding/none.hpp" FOLDER "encoding/none")
lingo_target_sources(lingo INTERFACE "encoding/utf8.hpp" FOLDER "encoding/utf")

lingo_target_sources(lingo INTERFACE "encoding/char.hpp" FOLDER "encoding/char")
lingo_target_sources(lingo INTERFACE "encoding/point_iterator.hpp" FOLDER "encoding/iterators")
lingo_target_sources(lingo INTERFACE "encoding/result.hpp" FOLDER "encoding/result")

# Error handlers
lingo_target_sources(lingo INTERFACE "error/error_code.hpp" FOLDER "error")
lingo_target_sources(lingo INTERFACE "error/exception.hpp" FOLDER "error")

lingo_target_sources(lingo INTERFACE "error/replace.hpp" FOLDER "error")
lingo_target_sources(lingo INTERFACE "error/skip.hpp" FOLDER "error")
lingo_target_sources(lingo INTERFACE "error/strict.hpp" FOLDER "error")

# Utility
lingo_target_sources(lingo INTERFACE "compressed_pair.hpp" FOLDER "utility")



# Create IDE project
get_target_property(LINGO_SOURCES lingo INTERFACE_SOURCES)
add_custom_target(lingo_ide SOURCES ${LINGO_SOURCES})
lingo_target_folder(lingo_ide Libraries)
