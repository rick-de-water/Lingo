# Create library
lingo_add_library(lingo)
lingo_target_folder(lingo Libraries)

# Add dependencies
target_include_directories(lingo PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")

# Add sources
lingo_target_sources(lingo PRIVATE "constexpr.hpp" "strlen.hpp" "standard_typedefs.hpp")
lingo_target_sources(lingo PRIVATE "string.hpp" "string_storage.hpp" FOLDER "string")
lingo_target_sources(lingo PRIVATE "string_view.hpp" "string_view_storage.hpp" FOLDER "string_view")

# Character sets
lingo_target_sources(lingo PRIVATE "set/ascii.hpp" FOLDER "set/ascii")
lingo_target_sources(lingo PRIVATE "set/unicode.hpp" FOLDER "set/unicode")

# Encoding
lingo_target_sources(lingo PRIVATE "encoding/none.hpp" FOLDER "encoding/none")
lingo_target_sources(lingo PRIVATE "encoding/utf_iterator.hpp" "encoding/utf8.hpp" FOLDER "encoding/utf")

file(TOUCH "${CMAKE_CURRENT_BINARY_DIR}/_.cpp")
lingo_target_sources(lingo PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/_.cpp")