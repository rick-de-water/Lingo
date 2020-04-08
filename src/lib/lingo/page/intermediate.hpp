#ifndef H_LINGO_PAGE_INTERMEDIATE
#define H_LINGO_PAGE_INTERMEDIATE

#include <lingo/page/unicode.hpp>

namespace lingo
{
	namespace page
	{
		// Default to unicode as an intermediate page
		template <typename SourcePage, typename DestinationPage, typename Enable = void>
		struct intermediate
		{
			using type = unicode_default;
		};

		template <typename SourcePage, typename DestinationPage>
		using intermediate_t = typename intermediate<SourcePage, DestinationPage>::type;
	}
}

#endif