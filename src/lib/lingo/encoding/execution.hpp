#ifndef H_LINGO_ENCODING_EXECUTION
#define H_LINGO_ENCODING_EXECUTION

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
		struct execution_encoding
		{
			using type = none<Unit, char32_t>;
		};

		template <typename Unit>
		struct execution_encoding<Unit, typename std::enable_if<sizeof(Unit) == 1>::type>
		{
			using type = utf8<Unit, char32_t>;
		};

		template <typename Unit>
		struct execution_encoding<Unit, typename std::enable_if<sizeof(Unit) == 2>::type>
		{
			using type = utf16<Unit, char32_t>;
		};

		template <typename Unit>
		struct execution_encoding<Unit, typename std::enable_if<sizeof(Unit) == 4>::type>
		{
			using type = utf32<Unit, char32_t>;
		};

		#ifdef LINGO_CHAR_ENCODING
		template <>
		struct execution_encoding<char, void>
		{
			using type = LINGO_CHAR_ENCODING<char, char32_t>;
		};
		template <>
		struct execution_encoding<signed char, void>
		{
			using type = LINGO_CHAR_ENCODING<signed char, char32_t>;
		};
		template <>
		struct execution_encoding<unsigned char, void>
		{
			using type = LINGO_CHAR_ENCODING<unsigned char, char32_t>;
		};
		#endif

		#ifdef LINGO_WCHAR_ENCODING
		template <>
		struct execution_encoding<wchar_t, void>
		{
			using type = LINGO_WCHAR_ENCODING<wchar_t, char32_t>;
		};
		#endif

		template <typename Unit>
		using execution_encoding_t = typename execution_encoding<Unit>::type;
	}
}

#endif