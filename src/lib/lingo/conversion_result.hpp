#ifndef H_LINGO_CONVERSION_RESULT
#define H_LINGO_CONVERSION_RESULT

#include <cstddef>

namespace lingo
{
	struct conversion_result
	{
		std::size_t units_read;
		std::size_t units_written;
	};
}

#endif