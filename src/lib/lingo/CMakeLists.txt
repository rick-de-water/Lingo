# Create library
lingo_add_library(lingo)
lingo_target_folder(lingo Libraries)

target_compile_features(lingo INTERFACE cxx_std_11)

# Add settings
if (MSVC)
	# constexpr if warnings are buggy and get false positives
	target_compile_options(lingo PUBLIC /wd4127 /wd4984)
endif()

# Add dependencies
target_include_directories(lingo PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")

# Add sources
lingo_target_sources(lingo PRIVATE "constexpr.hpp" "strlen.hpp")

# Natvis
if (MSVC)
	lingo_target_sources(lingo PUBLIC "lingo.natvis" FOLDER "natvis")
endif()

# Strings
lingo_target_sources(lingo PRIVATE "string.hpp" "string_storage.hpp" FOLDER "strings/string")
lingo_target_sources(lingo PRIVATE "string_view.hpp" "string_view_storage.hpp" FOLDER "strings/string_view")

# Code pages
lingo_target_sources(lingo PRIVATE "page/ascii.hpp" FOLDER "page/ascii")
lingo_target_sources(lingo PRIVATE "page/unicode.hpp" "page/unicode.cpp" FOLDER "page/unicode")

lingo_target_sources(lingo PRIVATE "page/result.hpp" FOLDER "page/result")

# EncodingT
lingo_target_sources(lingo PRIVATE "encoding/none.hpp" FOLDER "encoding/none")
lingo_target_sources(lingo PRIVATE "encoding/utf8.hpp" FOLDER "encoding/utf")

lingo_target_sources(lingo PRIVATE "encoding/char.hpp" FOLDER "encoding/char")
lingo_target_sources(lingo PRIVATE "encoding/point_iterator.hpp" FOLDER "encoding/iterators")
lingo_target_sources(lingo PRIVATE "encoding/result.hpp" FOLDER "encoding/result")

# Error handlers
lingo_target_sources(lingo PRIVATE "error/error_code.hpp" FOLDER "error")
lingo_target_sources(lingo PRIVATE "error/exception.hpp" FOLDER "error")

lingo_target_sources(lingo PRIVATE "error/replace.hpp" FOLDER "error")
lingo_target_sources(lingo PRIVATE "error/skip.hpp" FOLDER "error")
lingo_target_sources(lingo PRIVATE "error/strict.hpp" FOLDER "error")

# Utility
lingo_target_sources(lingo PRIVATE "compressed_pair.hpp" FOLDER "utility")