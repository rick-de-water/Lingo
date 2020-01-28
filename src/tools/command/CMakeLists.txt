# Create executable
add_executable(command)
set_target_properties(command PROPERTIES FOLDER Tools)
set_target_properties(command PROPERTIES OUTPUT_NAME "lingo")

# Add dependencies
target_link_libraries(command lingo)

# Add sources
list(APPEND COMMAND_MANUAL_SOURCES "main.cpp")

target_sources(command PRIVATE ${COMMAND_MANUAL_SOURCES} ${COMMAND_GENERATED_SOURCES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${COMMAND_MANUAL_SOURCES})
source_group(TREE "${CMAKE_CURRENT_BINARY_DIR}" FILES ${COMMAND_GENERATED_SOURCES})

# Install
install(TARGETS command EXPORT lingo)