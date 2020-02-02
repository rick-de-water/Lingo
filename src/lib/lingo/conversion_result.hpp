#ifndef H_LINGO_CONVERSION_RESULT
#define H_LINGO_CONVERSION_RESULT

#include <cstddef>

namespace lingo
{
	struct conversion_result
	{
		std::size_t source_read;
		std::size_t destination_written;
	};
}

#endif