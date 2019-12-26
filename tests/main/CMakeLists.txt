# Create library
lingo_add_library(test_main STATIC)
lingo_target_folder(test_main Tests)

# Add dependencies
target_link_libraries(test_main catch)

# Add sources
lingo_target_sources(test_main PRIVATE "main.cpp" FOLDER "Main")