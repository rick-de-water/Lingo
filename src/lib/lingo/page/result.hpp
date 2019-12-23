#ifndef H_LINGO_PAGE_RESULT
#define H_LINGO_PAGE_RESULT

#include <lingo/error/error_code.hpp>

#include <cstddef>

namespace lingo
{
	namespace page
	{
		template <typename Point>
		struct map_result
		{
			Point point;
			error::error_code error;
		};
	}
}

#endif