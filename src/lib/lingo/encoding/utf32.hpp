#ifndef H_LINGO_ENCODING_UTF32
#define H_LINGO_ENCODING_UTF32

#include <lingo/platform/constexpr.hpp>

#include <lingo/encoding/endian.hpp>
#include <lingo/encoding/result.hpp>
#include <lingo/encoding/internal/bit_converter.hpp>

#include <climits>
#include <cstddef>

namespace lingo
{
	namespace encoding
	{
		template <typename Unit, typename Point>
		struct utf32
		{
			public:
			using unit_type = Unit;
			using point_type = Point;

			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;

			static LINGO_CONSTEXPR11 size_type max_units = 1;
			static LINGO_CONSTEXPR11 size_type min_unit_bits = 21;
			static LINGO_CONSTEXPR11 size_type min_point_bits = 21;

			using encode_result_type = encode_result<unit_type, point_type>;
			using decode_result_type = decode_result<unit_type, point_type>;
			using encode_source_type = typename encode_result_type::source_type;
			using decode_source_type = typename decode_result_type::source_type;
			using encode_destination_type = typename encode_result_type::destination_type;
			using decode_destination_type = typename decode_result_type::destination_type;

			struct encode_state_type {};
			struct decode_state_type {};

			private:
			using bit_converter_type = internal::bit_converter<unit_type, min_unit_bits, point_type, min_point_bits>;
			using unit_bits_type = typename bit_converter_type::unit_bits_type;
			using point_bits_type = typename bit_converter_type::point_bits_type;

			public:
			static LINGO_CONSTEXPR14 encode_result_type encode_one(encode_source_type source, encode_destination_type destination, encode_state_type&) noexcept
			{
				return encode_one(source, destination);
			}

			static LINGO_CONSTEXPR14 encode_result_type encode_one(encode_source_type source, encode_destination_type destination) noexcept
			{
				// Check if there is at least 1 point
				if (source.size() > 0)
				{
					return { source, destination, error::error_code::source_buffer_too_small };
				}

				// Convert to bits
				const point_bits_type point_bits = bit_converter_type::to_point_bits(source[0]);

				// Reject points beyond 0x10FFFF
				LINGO_IF_CONSTEXPR(sizeof(point_bits_type) * CHAR_BIT > min_point_bits)
				{
					if (point_bits > 0x10FFFF)
					{
						return { source, destination, error::error_code::invalid_point };
					}
				}

				// Reject surrogates
				if ((point_bits >= 0xD800 && point_bits < 0xE000))
				{
					return { source, destination, error::error_code::invalid_point };
				}

				// Check if the destination buffer is large enough
				if (destination.size() < 1)
				{
					return { source, destination, error::error_code::destination_buffer_too_small };
				}

				// Return result
				const unit_bits_type unit_bits = static_cast<unit_bits_type>(point_bits);
				const unit_type unit = bit_converter_type::from_unit_bits(unit_bits);
				destination[0] = unit;
				return { source.subspan(1), destination.subspan(1), error::error_code::success };
			}

			static LINGO_CONSTEXPR14 decode_result_type decode_one(decode_source_type source, decode_destination_type destination, decode_state_type&) noexcept
			{
				return decode_one(source, destination);
			}

			static LINGO_CONSTEXPR14 decode_result_type decode_one(decode_source_type source, decode_destination_type destination) noexcept
			{
				// Check if there is at least 1 unit
				if (source.size() == 0)
				{
					return { source, destination, error::error_code::source_buffer_too_small };
				}

				// Convert to bits
				const unit_bits_type unit_bits = bit_converter_type::to_unit_bits(source[0]);

				// Reject points beyond 0x10FFFF
				LINGO_IF_CONSTEXPR(std::numeric_limits<unit_bits_type>::digits > min_unit_bits)
				{
					if (unit_bits > 0x10FFFF)
					{
						return { source, destination, error::error_code::invalid_unit };
					}
				}

				// Reject surrogates
				if ((unit_bits >= 0xD800 && unit_bits < 0xE000))
				{
					return { source, destination, error::error_code::invalid_unit };
				}

				// Check if the destination buffer is large enough
				if (destination.size() < 1)
				{
					return { source, destination, error::error_code::destination_buffer_too_small };
				}

				// Return result
				const point_bits_type point_bits = static_cast<point_bits_type>(unit_bits);
				destination[0] = bit_converter_type::from_point_bits(point_bits);
				return { source.subspan(1), destination.subspan(1), error::error_code::success };
			}
		};

		#if LINGO_ARCHITECTURE_ENDIANNESS == LINGO_ARCHITECTURE_LITTLE_ENDIAN
		template <typename Unit, typename Point>
		using utf32_le = utf32<Unit, Point>;
		template <typename Unit, typename Point>
		using utf32_be = swap_endian<utf32<Unit, Point>>;
		#else
		template <typename Unit, typename Point>
		using utf32_le = swap_endian<utf32<Unit, Point>>;
		template <typename Unit, typename Point>
		using utf32_be = utf32<Unit, Point>;
		#endif
	}
}

#endif