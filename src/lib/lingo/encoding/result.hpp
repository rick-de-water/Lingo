#ifndef H_LINGO_ENCODING_RESULT
#define H_LINGO_ENCODING_RESULT

#include <lingo/error/error_code.hpp>

#include <cstddef>

namespace lingo
{
	namespace encoding
	{
		struct encode_result
		{
			std::size_t source_read;
			std::size_t destination_written;
			error::error_code error;
		};

		struct decode_result
		{
			std::size_t source_read;
			std::size_t destination_written;
			error::error_code error;
		};
	}
}

#endif