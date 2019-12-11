# Create executable
lingo_add_executable(test_lingo)
lingo_target_folder(test_lingo Tests)

# Add dependencies
target_link_libraries(test_lingo test_main)

# Add sources
# Encoding
lingo_target_sources(test_lingo PRIVATE "encoding/utf8.cpp" FOLDER "encoding/utf8")

# Strings
lingo_target_sources(test_lingo PRIVATE "string.cpp" FOLDER "strings/string")
lingo_target_sources(test_lingo PRIVATE "string_view.cpp" FOLDER "strings/string_view")

# Discover tests
catch_discover_tests(test_lingo)