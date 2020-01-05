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

		#ifdef LINGO_CHAR_ENCODING
		template <>
		struct cstring_default_encoding<char, void>
		{
			using type = LINGO_CHAR_ENCODING<char, char32_t>;
		};
		template <>
		struct cstring_default_encoding<signed char, void>
		{
			using type = LINGO_CHAR_ENCODING<signed char, char32_t>;
		};
		template <>
		struct cstring_default_encoding<unsigned char, void>
		{
			using type = LINGO_CHAR_ENCODING<unsigned char, char32_t>;
		};
		#endif

		#ifdef LINGO_WCHAR_ENCODING
		template <>
		struct cstring_default_encoding<wchar_t, void>
		{
			using type = LINGO_WCHAR_ENCODING<wchar_t, char32_t>;
		};
		#endif

		template <typename Unit>
		using cstring_default_encoding_t = typename cstring_default_encoding<Unit>::type;

	}
}

#endif