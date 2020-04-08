#ifndef H_LINGO_PAGE_EXECUTION
#define H_LINGO_PAGE_EXECUTION

#include <lingo/page/ascii.hpp>
#include <lingo/page/iso_8859.hpp>
#include <lingo/page/unicode.hpp>

namespace lingo
{
	namespace page
	{
		template <typename Unit>
		struct execution_page
		{
			using type = unicode_default;
		};

		#ifdef LINGO_CHAR_PAGE
		template <>
		struct execution_page<char>
		{
			using type = LINGO_CHAR_PAGE;
		};
		template <>
		struct execution_page<signed char>
		{
			using type = LINGO_CHAR_PAGE;
		};
		template <>
		struct execution_page<unsigned char>
		{
			using type = LINGO_CHAR_PAGE;
		};
		#endif

		#ifdef LINGO_WCHAR_PAGE
		template <>
		struct execution_page<wchar_t>
		{
			using type = LINGO_WCHAR_PAGE;
		};
		#endif

		template <typename Unit>
		using execution_page_t = typename execution_page<Unit>::type;
	}
}

#endif