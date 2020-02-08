#ifndef H_LINGO_ENCODING_ENDIAN
#define H_LINGO_ENCODING_ENDIAN

#include <lingo/platform/constexpr.hpp>
#include <lingo/platform/endian.hpp>

#include <lingo/encoding/result.hpp>

#include <algorithm>

namespace lingo
{
	namespace encoding
	{
		template <typename Unit>
		struct swap_endian
		{
			using unit_type = Unit;
			using point_type = unit_type;

			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;

			static LINGO_CONSTEXPR11 size_type max_units = 1;
			static LINGO_CONSTEXPR11 size_type min_unit_bits = sizeof(unit_type) * CHAR_BIT;
			static LINGO_CONSTEXPR11 size_type min_point_bits = min_unit_bits;

			using encode_result_type = encode_result<unit_type, point_type>;
			using decode_result_type = decode_result<unit_type, point_type>;
			using encode_source_type = typename encode_result_type::source_type;
			using decode_source_type = typename decode_result_type::source_type;
			using encode_destination_type = typename encode_result_type::destination_type;
			using decode_destination_type = typename decode_result_type::destination_type;

			struct encode_state_type {};
			struct decode_state_type {};

			static LINGO_CONSTEXPR14 encode_result_type encode_one(encode_source_type source, encode_destination_type destination, encode_state_type&, bool) noexcept
			{
				return encode_one(source, destination);
			}

			static LINGO_CONSTEXPR14 encode_result_type encode_one(encode_source_type source, encode_destination_type destination) noexcept
			{
				if (source.size() < 1)
				{
					return { source, destination, error::error_code::source_buffer_too_small };
				}

				if (destination.size() < 1)
				{
					return { source, destination, error::error_code::destination_buffer_too_small };
				}

				destination[0] = platform::swap_endian(source[0]);
				return { source.subspan(1), destination.subspan(1), error::error_code::success };
			}

			static LINGO_CONSTEXPR14 decode_result_type decode_one(decode_source_type source, decode_destination_type destination, decode_state_type&, bool) noexcept
			{
				return decode_one(source, destination);
			}

			static LINGO_CONSTEXPR14 decode_result_type decode_one(decode_source_type source, decode_destination_type destination) noexcept
			{
				if (source.size() < 1)
				{
					return { source, destination, error::error_code::source_buffer_too_small };
				}

				if (destination.size() < 1)
				{
					return { source, destination, error::error_code::destination_buffer_too_small };
				}

				destination[0] = platform::swap_endian(source[0]);
				return { source.subspan(1), destination.subspan(1), error::error_code::success };
			}
		};
	}
}

#endif