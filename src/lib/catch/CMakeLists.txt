add_library(catch INTERFACE)

if (MSVC)
	# Unnamed objects with custom construction
	target_compile_options(catch INTERFACE /wd26444)
endif()

target_include_directories(catch INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/..")