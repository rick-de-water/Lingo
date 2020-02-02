#ifndef H_LINGO_ENCODING_RESULT
#define H_LINGO_ENCODING_RESULT

#include <lingo/error/error_code.hpp>

#include <lingo/utility/span.hpp>

#include <cstddef>

namespace lingo
{
	namespace encoding
	{
		template <typename Unit, typename Point>
		struct encode_result
		{
			using unit_type = Unit;
			using point_type = Point;

			using source_type = utility::span<const point_type>;
			using destination_type = utility::span<unit_type>;

			source_type source;
			destination_type destination;
			error::error_code error;
		};

		template <typename Unit, typename Point>
		struct decode_result
		{
			using unit_type = Unit;
			using point_type = Point;

			using source_type = utility::span<const unit_type>;
			using destination_type = utility::span<point_type>;

			source_type source;
			destination_type destination;
			error::error_code error;
		};
	}
}

#endif