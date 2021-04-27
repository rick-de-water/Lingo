#ifndef H_LINGO_TEST_TYPES
#define H_LINGO_TEST_TYPES

#include <lingo/platform/wchar.hpp>

#include "tuple_matrix.hpp"

#include <cstdint>
#include <cstddef>
#include <tuple>
#include <utility>

namespace lingo
{
	namespace test
	{
		/*using unit_types = std::tuple<
			char,
			#ifdef __cpp_char8_t
			char8_t,
			#endif
			wchar_t,
			char16_t,
			char32_t

			#ifndef _DEBUG
			signed char,
			unsigned char,
			int_least8_t,
			int_least16_t,
			int_least32_t
			int_least64_t
			uint_least8_t,
			uint_least16_t,
			uint_least32_t,
			uint_least64_t
			#endif
		>;*/

		using unit_least_64_types = std::tuple<
			#if WCHAR_BITS >= 64
			wchar_t
			#endif
		>;

		using unit_least_32_types = tuple_concat_t<
			std::tuple<
				#if WCHAR_BITS >= 32 && WCHAR_BITS < 64
				wchar_t,
				#endif
				char32_t>,
			unit_least_64_types>;
		
		using unit_least_16_types = tuple_concat_t<
			std::tuple<
				#if WCHAR_BITS >= 16 && WCHAR_BITS < 32
				wchar_t,
				#endif
				char16_t>,
			unit_least_32_types>;
		
		using unit_least_8_types = tuple_concat_t<
			std::tuple<
				#if WCHAR_BITS < 16
				wchar_t,
				#endif
				#ifdef __cpp_char8_t
				char8_t,
				#endif
				char>,
			unit_least_16_types>;

		using unit_types = unit_least_8_types;

		using unit_type_matrix = tuple_matrix_t<unit_types, unit_types>;
	}
}

#endif