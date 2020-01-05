#ifndef H_LINGO_PAGE_CSTRING
#define H_LINGO_PAGE_CSTRING

#include <lingo/page/ascii.hpp>
#include <lingo/page/iso_8859.hpp>
#include <lingo/page/unicode.hpp>

namespace lingo
{
	namespace page
	{
		template <typename Unit>
		struct cstring_default_page
		{
			using type = unicode;
		};

		#ifdef LINGO_CHAR_PAGE
		template <>
		struct cstring_default_page<char>
		{
			using type = LINGO_CHAR_PAGE;
		};
		template <>
		struct cstring_default_page<signed char>
		{
			using type = LINGO_CHAR_PAGE;
		};
		template <>
		struct cstring_default_page<unsigned char>
		{
			using type = LINGO_CHAR_PAGE;
		};
		#endif

		#ifdef LINGO_WCHAR_PAGE
		template <>
		struct cstring_default_page<wchar_t>
		{
			using type = LINGO_WCHAR_PAGE;
		};
		#endif

		template <typename Unit>
		using cstring_default_page_t = typename cstring_default_page<Unit>::type;
	}
}

#endif