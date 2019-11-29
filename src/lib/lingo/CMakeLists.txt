add_library(lingo)
target_include_directories(lingo PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")

target_sources(lingo PRIVATE string.hpp string_view.hpp string_view_storage.hpp functions.hpp)



file(TOUCH "${CMAKE_CURRENT_BINARY_DIR}/_.cpp")
target_sources(lingo PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/_.cpp")