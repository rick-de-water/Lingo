# Create library
lingo_add_library(lingo)
lingo_target_folder(lingo Libraries)

target_compile_features(lingo PUBLIC cxx_std_11)

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
lingo_target_sources(lingo PUBLIC "lingo.natvis" FOLDER "natvis")

# Strings
lingo_target_sources(lingo PRIVATE "string.hpp" "string_storage.hpp" FOLDER "strings/string")
lingo_target_sources(lingo PRIVATE "string_view.hpp" "string_view_storage.hpp" FOLDER "strings/string_view")

# Character sets
lingo_target_sources(lingo PRIVATE "set/ascii.hpp" FOLDER "set/ascii")
lingo_target_sources(lingo PRIVATE "set/unicode.hpp" FOLDER "set/unicode")

# Encoding
lingo_target_sources(lingo PRIVATE "encoding/none.hpp" FOLDER "encoding/none")
lingo_target_sources(lingo PRIVATE "encoding/utf_iterator.hpp" "encoding/utf8.hpp" FOLDER "encoding/utf")

# Error handlers
lingo_target_sources(lingo PRIVATE "error/error_code.hpp" FOLDER "error")
lingo_target_sources(lingo PRIVATE "error/replace.hpp" FOLDER "error")
lingo_target_sources(lingo PRIVATE "error/skip.hpp" FOLDER "error")
lingo_target_sources(lingo PRIVATE "error/strict.hpp" FOLDER "error")

file(TOUCH "${CMAKE_CURRENT_BINARY_DIR}/_.cpp")
lingo_target_sources(lingo PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/_.cpp")