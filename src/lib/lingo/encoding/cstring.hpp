#ifndef H_LINGO_ENCODING_CSTRING
#define H_LINGO_ENCODING_CSTRING

#include <lingo/encoding/none.hpp>
#include <lingo/encoding/utf8.hpp>
#include <lingo/encoding/utf16.hpp>
#include <lingo/encoding/utf32.hpp>

#include <type_traits>

namespace lingo
{
	namespace encoding
	{
		template <typename Unit, typename Enable = void>
		struct cstring_default_encoding
		{
			using type = none<Unit, char32_t>;
		};

		template <typename Unit>
		struct cstring_default_encoding<Unit, typename std::enable_if<sizeof(Unit) == 1>::type>
		{
			using type = utf8<Unit, char32_t>;
		};

		template <typename Unit>
		struct cstring_default_encoding<Unit, typename std::enable_if<sizeof(Unit) == 2>::type>
		{
			using type = utf16<Unit, char32_t>;
		};

		template <typename Unit>
		struct cstring_default_encoding<Unit, typename std::enable_if<sizeof(Unit) == 4>::type>
		{
			using type = utf32<Unit, char32_t>;
		};

		template <typename Unit>
		using cstring_default_encoding_t = typename cstring_default_encoding<Unit>::type;
	}
}

#endif