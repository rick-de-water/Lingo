function(unicode_parse_mapping SPEC POINTS_VARIABLE UNICODE_POINTS_VARIABLE UNICODE_NAMES_VARIABLE)
	set(MAPPING_REGEX "(0x[0-9A-F]+)\t(0x[0-9A-F]+)\t#\t([^\n]+)")

	file(READ "${SPEC}" SPEC_DATA)
	string(REGEX MATCHALL "${MAPPING_REGEX}" LINES "${SPEC_DATA}")

	foreach(LINE ${LINES})
		string(REGEX REPLACE "${MAPPING_REGEX}" "\\1" POINT "${LINE}")
		string(REGEX REPLACE "${MAPPING_REGEX}" "\\2" UNICODE_POINT "${LINE}")
		string(REGEX REPLACE "${MAPPING_REGEX}" "\\3" UNICODE_NAME "${LINE}")
		list(APPEND POINTS "${POINT}")
		list(APPEND UNICODE_POINTS "${UNICODE_POINT}")
		list(APPEND UNICODE_NAMES "${UNICODE_NAME}")
	endforeach()

	set("${POINTS_VARIABLE}" "${POINTS}" PARENT_SCOPE)
	set("${UNICODE_POINTS_VARIABLE}" "${UNICODE_POINTS}" PARENT_SCOPE)
	set("${UNICODE_NAMES_VARIABLE}" "${UNICODE_NAMES}" PARENT_SCOPE)

endfunction()

function(unicode_list_to_c_array OUTPUT_VARIABLE)
	set(OUTPUT "{ ")
	foreach(POINT ${ARGN})
		string(APPEND OUTPUT "${POINT}, ")
	endforeach()
	string(APPEND OUTPUT "}")
	
	set("${OUTPUT_VARIABLE}" "${OUTPUT}" PARENT_SCOPE)
endfunction()

function(unicode_generate_mapping_header FROM TO FROM_NAME TO_NAME OUTPUT_VARIABLE)
	# Create an empty minor table as a starting point
	set(MINOR_TABLE_SIZE 256)
	set(MINOR_TABLE_RANGE 255)
	foreach(POINT_INDEX RANGE ${MINOR_TABLE_RANGE})
		list(APPEND EMPTY_MINOR_TABLE "0xFFFF")
	endforeach()

	# Dereference FROM and TO
	set(FROM "${${FROM}}")
	set(TO "${${TO}}")

	# Find the smallest and the largest index
	set(POINT_INDEX_START 99999999)
	set(POINT_INDEX_END -99999999)
	foreach(INDEX ${FROM})
		if (${INDEX} GREATER POINT_INDEX_END)
			set(POINT_INDEX_END ${INDEX})
		endif()
		if (${INDEX} LESS POINT_INDEX_START)
			set(POINT_INDEX_START ${INDEX})
		endif()
	endforeach()
	list(LENGTH FROM POINTINDEX_COUNT)
	math(EXPR POINT_INDEX_RANGE "${POINTINDEX_COUNT} - 1")
	
	# Calculate which minor tables need to be created
	math(EXPR MINOR_TABLE_START "${POINT_INDEX_START} / ${MINOR_TABLE_SIZE}")
	math(EXPR MINOR_TABLE_END "${POINT_INDEX_END} / ${MINOR_TABLE_SIZE} + 1")
	math(EXPR MINOR_TABLE_COUNT "${MINOR_TABLE_END} - ${MINOR_TABLE_START}")
	math(EXPR MINOR_TABLE_RANGE "${MINOR_TABLE_COUNT} - 1")

	# Generate tables
	foreach(POINT_INDEX RANGE ${POINT_INDEX_RANGE})
		list(GET FROM ${POINT_INDEX} VALUE_FROM)
		list(GET TO ${POINT_INDEX} VALUE_TO)

		math(EXPR MINOR_TABLE_INDEX "${VALUE_FROM} / ${MINOR_TABLE_SIZE}")
		math(EXPR VALUE_INDEX "${VALUE_FROM} - (${MINOR_TABLE_INDEX} * ${MINOR_TABLE_SIZE})")

		if (NOT DEFINED "MINOR_TABLE_${MINOR_TABLE_INDEX}")
			set("MINOR_TABLE_${MINOR_TABLE_INDEX}" "${EMPTY_MINOR_TABLE}")
		endif()

		list(TRANSFORM "MINOR_TABLE_${MINOR_TABLE_INDEX}" REPLACE ".+" "${VALUE_TO}" AT "${VALUE_INDEX}")
	endforeach()

	# Generate c++ header
	string(TOUPPER "H_LINGO_PAGE_INTERNAL_${FROM_NAME}_${TO_NAME}_MAPPING" HEADER_GUARD)

	set(HEADER "#ifndef ${HEADER_GUARD}\n#define ${HEADER_GUARD}\n\n")
	string(APPEND HEADER "#include <cstdint>\n\n")
	string(APPEND HEADER "namespace lingo\n{\n")
	string(APPEND HEADER "\tnamespace page\n\t{\n")
	string(APPEND HEADER "\t\tnamespace internal\n\t\t{\n")
	string(APPEND HEADER "\t\t\ttemplate <typename T>\n")
	string(APPEND HEADER "\t\t\tstruct ${FROM_NAME}_${TO_NAME}_mapping\n\t\t\t{\n")

	unicode_list_to_c_array(MINOR_TABLE_ARRAY ${EMPTY_MINOR_TABLE})
	string(APPEND HEADER "\t\t\t\tstatic constexpr T minor_table_empty[${MINOR_TABLE_SIZE}] = ${MINOR_TABLE_ARRAY};\n")

	foreach(MINOR_TABLE_INDEX RANGE ${MINOR_TABLE_RANGE})
		if (DEFINED "MINOR_TABLE_${MINOR_TABLE_INDEX}")
			unicode_list_to_c_array(MINOR_TABLE_ARRAY ${MINOR_TABLE_${MINOR_TABLE_INDEX}})
			string(APPEND HEADER "\t\t\t\tstatic constexpr T minor_table_${MINOR_TABLE_INDEX}[${MINOR_TABLE_SIZE}] = ${MINOR_TABLE_ARRAY};\n")
		endif()
	endforeach()
	string(APPEND HEADER "\n")

	string(APPEND HEADER "\t\t\t\tstatic constexpr T point_start_index = ${POINT_INDEX_START};\n")
	string(APPEND HEADER "\t\t\t\tstatic constexpr T point_end_index = ${POINT_INDEX_END};\n\n")

	string(APPEND HEADER "\t\t\t\tstatic constexpr T minor_table_start_index = ${MINOR_TABLE_START};\n")
	string(APPEND HEADER "\t\t\t\tstatic constexpr T minor_table_end_index = ${MINOR_TABLE_END};\n")
	string(APPEND HEADER "\t\t\t\tstatic constexpr T minor_table_count = ${MINOR_TABLE_COUNT};\n\n")

	string(APPEND HEADER "\t\t\t\tstatic constexpr const T* major_table[${MINOR_TABLE_COUNT}] = { ")
	foreach(MINOR_TABLE_INDEX RANGE ${MINOR_TABLE_RANGE})
		if (DEFINED "MINOR_TABLE_${MINOR_TABLE_INDEX}")
			string(APPEND HEADER "minor_table_${MINOR_TABLE_INDEX}, ")
		else()
			string(APPEND HEADER "minor_table_empty, ")
		endif()
	endforeach()
	string(APPEND HEADER "};\n")

	string(APPEND HEADER "\t\t\t};\n")
	string(APPEND HEADER "\n")
	string(APPEND HEADER "\t\t\ttemplate <typename T> constexpr T ${FROM_NAME}_${TO_NAME}_mapping<T>::minor_table_empty[${MINOR_TABLE_SIZE}];\n")
	foreach(MINOR_TABLE_INDEX RANGE ${MINOR_TABLE_RANGE})
		if (DEFINED "MINOR_TABLE_${MINOR_TABLE_INDEX}")
			unicode_list_to_c_array(MINOR_TABLE_ARRAY ${MINOR_TABLE_${MINOR_TABLE_INDEX}})
			string(APPEND HEADER "\t\t\ttemplate <typename T> constexpr T ${FROM_NAME}_${TO_NAME}_mapping<T>::minor_table_${MINOR_TABLE_INDEX}[${MINOR_TABLE_SIZE}];\n")
		endif()
	endforeach()
	string(APPEND HEADER "\t\t\ttemplate <typename T> constexpr const T* ${FROM_NAME}_${TO_NAME}_mapping<T>::major_table[${MINOR_TABLE_COUNT}];\n")
	string(APPEND HEADER "\t\t}\n")
	string(APPEND HEADER "\t}\n")
	string(APPEND HEADER "}\n\n")
	string(APPEND HEADER "#endif")

	set("${OUTPUT_VARIABLE}" "${HEADER}" PARENT_SCOPE)
endfunction()

function(unicode_generate_headers SPEC NAME)
	unicode_parse_mapping("${SPEC}" POINTS UNICODE_POINTS NAMES)
	unicode_generate_mapping_header(POINTS UNICODE_POINTS "${NAME}" unicode FROM_HEADER)
	unicode_generate_mapping_header(UNICODE_POINTS POINTS unicode "${NAME}" TO_HEADER)

	file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/page/internal/${NAME}_unicode_mapping.hpp" "${FROM_HEADER}")
	file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/page/internal/unicode_${NAME}_mapping.hpp" "${TO_HEADER}")
endfunction()