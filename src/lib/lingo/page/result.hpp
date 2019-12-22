#ifndef H_LINGO_PAGE_RESULT
#define H_LINGO_PAGE_RESULT

#include <lingo/error/error_code.hpp>

#include <cstddef>

namespace lingo
{
	namespace page
	{
		struct to_unicode_result
		{
			char32_t point;
			error::error_code error;
		};

		template <typename Point>
		struct from_unicode_result
		{
			Point point;
			error::error_code error;
		};
	}
}

#endif