#ifndef H_LINGO_PAGE_CSTRING
#define H_LINGO_PAGE_CSTRING

#include <lingo/page/ascii.hpp>
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

		template <typename Unit>
		using cstring_default_page_t = typename cstring_default_page<Unit>::type;
	}
}

#endif