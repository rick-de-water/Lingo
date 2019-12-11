function(lingo_add_target TARGET)
	# Manually set language to C++
	set_target_properties("${TARGET}" PROPERTIES LINKER_LANGUAGE CXX)

	# Explicit exports only
	set_target_properties("${TARGET}" PROPERTIES CXX_VISIBILITY_PRESET hidden)

	# Unfiltered files
	source_group(" " REGULAR_EXPRESSION ".*")
	
	# CMake Rules folder
	source_group("Code Generation\\CMake Rules" REGULAR_EXPRESSION ".rule")
endfunction()

function(lingo_add_library TARGET)
	# Create library
	add_library("${TARGET}" ${ARGN})
	
	# Apply common settings to target
	lingo_add_target("${TARGET}")
endfunction()

function(lingo_add_executable TARGET)
	# Create executable
	add_executable("${TARGET}" ${ARGN} "")
	
	# Apply common settings to target
	lingo_add_target("${TARGET}")
endfunction()

function(lingo_target_folder TARGET FOLDER)
	set_target_properties("${TARGET}" PROPERTIES FOLDER "${FOLDER}")
endfunction()

function(lingo_target_sources TARGET)
	# Parse arguments
	set(OPTIONS)
	set(ONE_VALUE_ARGS FOLDER)
	set(MULTI_VALUE_ARGS)
	cmake_parse_arguments(SOURCES "${OPTIONS}" "${ONE_VALUE_ARGS}" "${MULTI_VALUE_ARGS}" ${ARGN})
	
	# Pass all unparsed arguments to target_sources
	target_sources("${TARGET}" ${SOURCES_UNPARSED_ARGUMENTS})

	# Apply folder
	if(DEFINED SOURCES_FOLDER)
		# Filter out keywords
		set(SOURCES ${SOURCES_UNPARSED_ARGUMENTS})
		list(FILTER SOURCES EXCLUDE REGEX "^PUBLIC$")
		list(FILTER SOURCES EXCLUDE REGEX "^PRIVATE$")
		list(FILTER SOURCES EXCLUDE REGEX "^INTERFACE$")

		# Apply folder to sources
		string(REPLACE "/" "\\" SOURCES_FOLDER "${SOURCES_FOLDER}")
		source_group("${SOURCES_FOLDER}" FILES ${SOURCES})
	endif()
endfunction()