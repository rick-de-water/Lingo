#ifndef H_LINGO_TEST_TYPES
#define H_LINGO_TEST_TYPES

#include "tuple_matrix.hpp"

#include <cstdint>
#include <cstddef>
#include <tuple>
#include <utility>

namespace lingo
{
	namespace test
	{
		using unit_types = std::tuple<
			char,
			#ifdef __cpp_char8_t
			char8_t,
			#endif
			//wchar_t,
			//char16_t,
			char32_t

			/*#ifndef _DEBUG
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
			#endif*/
		>;

		using unit_type_matrix = tuple_matrix_t<unit_types, unit_types>;
	}
}

#endif