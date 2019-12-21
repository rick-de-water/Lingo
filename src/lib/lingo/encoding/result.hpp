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
			std::size_t size;
			error::error_code error;
		};

		template <typename PointT>
		struct decode_result
		{
			PointT point;
			error::error_code error;
		};
	}
}

#endif