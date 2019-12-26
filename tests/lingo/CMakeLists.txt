# Create executable
lingo_add_executable(test_lingo)
lingo_target_folder(test_lingo Tests)

# Add settings
target_compile_definitions(test_lingo PRIVATE "LINGO_SPEC_DIRECTORY=\"${CMAKE_SOURCE_DIR}/spec\"")

# Add dependencies
target_link_libraries(test_lingo test_main lingo)
target_include_directories(test_lingo PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

# Add sources
lingo_target_sources(test_lingo PRIVATE "test_types.hpp" "test_case.hpp" "test_strings.hpp" FOLDER "test")

# Code pages
lingo_target_sources(test_lingo PRIVATE "page/ascii.cpp" FOLDER "page/ascii")
lingo_target_sources(test_lingo PRIVATE "page/iso_8859.cpp" FOLDER "page/iso_8859")
lingo_target_sources(test_lingo PRIVATE "page/unicode.cpp" FOLDER "page/unicode")

# Encoding
lingo_target_sources(test_lingo PRIVATE "encoding/utf8.cpp" FOLDER "encoding/utf8")

# Strings
lingo_target_sources(test_lingo PRIVATE "string.cpp" FOLDER "strings/string")
lingo_target_sources(test_lingo PRIVATE "string_view.cpp" FOLDER "strings/string_view")

# Discover tests
catch_discover_tests(test_lingo)