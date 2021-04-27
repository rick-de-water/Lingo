#ifndef H_LINGO_ENCODING_UTF8
#define H_LINGO_ENCODING_UTF8

#include <lingo/platform/constexpr.hpp>

#include <lingo/encoding/result.hpp>
#include <lingo/encoding/internal/bit_converter.hpp>

#include <cassert>
#include <climits>
#include <limits>
#include <type_traits>

namespace lingo
{
	namespace encoding
	{
		template <typename Unit, typename Point>
		struct utf8
		{
			public:
			using unit_type = Unit;
			using point_type = Point;

			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;

			static LINGO_CONSTEXPR11 size_type max_units = 4;
			static LINGO_CONSTEXPR11 size_type min_unit_bits = 8;
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

			static LINGO_CONSTEXPR11 unit_bits_type first_unit_prefix_markers[5] =
			{
				0x00,
				0x00,
				0xC0,
				0xE0,
				0xF0,
			};

			static LINGO_CONSTEXPR11 unit_bits_type first_unit_data_masks[5] =
			{
				0x7F,
				0x7F,
				0x1F,
				0x0F,
				0x07,
			};

			static LINGO_CONSTEXPR11 unit_bits_type continuation_unit_prefix_marker = 0x80;
			static LINGO_CONSTEXPR11 unit_bits_type continuation_unit_prefix_mask = 0xC0;
			static LINGO_CONSTEXPR11 unit_bits_type continuation_unit_data_mask = 0x3F;

			public:
			static LINGO_CONSTEXPR14 size_type point_size(point_type point) noexcept
			{
				// Convert to bits
				const point_bits_type point_bits = bit_converter_type::to_point_bits(point);

				// Reject values above 0x10FFFF
				LINGO_IF_CONSTEXPR(sizeof(point_bits_type) * CHAR_BIT > min_point_bits)
				{
					if (point_bits > 0x10FFFF)
					{
						return 0;
					}
				}

				// Calculate size
				if (point_bits < 0x80)
				{
					return 1;
				}
				else if (point_bits < 0x800)
				{
					return 2;
				}
				else if (point_bits < 0x10000)
				{
					return 3;
				}
				else
				{
					return 4;
				}
			}

			static LINGO_CONSTEXPR14 size_type unit_size(unit_type unit) noexcept
			{
				// Lookup table
				LINGO_CONSTEXPR11 size_type sizes[]
				{
					// First 128 bytes are single byte code points
					1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
					1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
					1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
					1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
					1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
					1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
					1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
					1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

					// Continuation bytes are invalid
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

					// 2 byte code points
					2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
					2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,

					// 3 byte code points
					3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,

					// 4 byte code points
					4, 4, 4, 4, 4, 4, 4, 4,

					// Code points longer than 4 bytes are invalid
					0, 0, 0, 0, 0, 0, 0, 0
				};

				// Convert to bits
				const unit_bits_type unit_bits = bit_converter_type::to_unit_bits(unit);

				// Reject values above 0xFF
				LINGO_IF_CONSTEXPR(sizeof(unit_bits_type) * CHAR_BIT > min_unit_bits)
				{
					if (unit_bits > 0xFF)
					{
						return 0;
					}
				}

				// Get size from lookup table
				return sizes[unit_bits];
			}

			static LINGO_CONSTEXPR14 encode_result_type encode_one(encode_source_type source, encode_destination_type destination, encode_state_type&, bool) noexcept
			{
				return encode_one(source, destination);
			}

			static LINGO_CONSTEXPR14 encode_result_type encode_one(encode_source_type source, encode_destination_type destination) noexcept
			{
				// Check if there is at least 1 point
				if (source.size() < 1)
				{
					return { source, destination, error::error_code::source_buffer_too_small };
				}

				// Calculate the number of units needed to encode this point
				const size_type required_size = point_size(source[0]);
				if (required_size == 0)
				{
					return { source, destination, error::error_code::invalid_point };
				}
				assert(required_size <= max_units);

				// Check if the destination buffer is large enough to fit the encoded point
				if (required_size > destination.size())
				{
					return { source, destination, error::error_code::destination_buffer_too_small };
				}

				// Encode the first unit
				point_bits_type point_bits = bit_converter_type::to_point_bits(source[0]);
				const size_type last_index = required_size - 1;
				const unit_bits_type first_unit_bits = first_unit_prefix_markers[required_size] | static_cast<unit_bits_type>(point_bits >> (6 * last_index));
				destination[0] = bit_converter_type::from_unit_bits(first_unit_bits);

				// Encode the subsequent units
				for (size_t i = 1; i < required_size; ++i)
				{
					const unit_bits_type continuation_unit_bits = continuation_unit_prefix_marker | static_cast<unit_bits_type>((point_bits >> (6 * (last_index - i))) & continuation_unit_data_mask);
					destination[i] = bit_converter_type::from_unit_bits(continuation_unit_bits);
				}

				// Return the result
				return { source.subspan(1), destination.subspan(required_size), error::error_code::success };
			}

			static LINGO_CONSTEXPR14 decode_result_type decode_one(decode_source_type source, decode_destination_type destination, decode_state_type&, bool) noexcept
			{
				return decode_one(source, destination);
			}

			static LINGO_CONSTEXPR14 decode_result_type decode_one(decode_source_type source, decode_destination_type destination) noexcept
			{
				// Check if there is at least one unit
				if (source.size() == 0)
				{
					return { source, destination, error::error_code::source_buffer_too_small };
				}

				// Calculate the size of the code point
				const std::size_t required_size = unit_size(source[0]);

				// Check if the first code unit is valid
				if (required_size == 0)
				{
					return { source, destination, error::error_code::invalid_unit };
				}
				assert(required_size <= max_units);

				// Check if there are enough code units left for the code point
				if (source.size() < required_size)
				{
					return { source, destination, error::error_code::source_buffer_too_small };
				}

				// Check if there is room for at least 1 point
				if (destination.size() < 1)
				{
					return { source, destination, error::error_code::destination_buffer_too_small };
				}

				// Get the bits from the first unit
				const unit_bits_type first_unit_bits = bit_converter_type::to_unit_bits(source[0]);
				point_bits_type point_bits = first_unit_bits & first_unit_data_masks[required_size];

				// Get the bits from all the continuation bytes
				for (std::size_t i = 1; i < required_size; ++i)
				{
					// Every continuation byte adds 6 bits of information, so we shift the code point to the left by 6
					point_bits <<= 6;

					// Check if this is a valid continuation byte
					const unit_bits_type continuation_unit_bits = bit_converter_type::to_unit_bits(source[i]);
					if ((continuation_unit_bits & continuation_unit_prefix_mask) != continuation_unit_prefix_marker)
					{
						return { source, destination, error::error_code::invalid_unit };
					}

					// Add the bits to the code point
					point_bits |= continuation_unit_bits & continuation_unit_data_mask;
				}

				// Store the point
				destination[0] = bit_converter_type::from_point_bits(point_bits);

				// Return the result
				return { source.subspan(required_size), destination.subspan(1), error::error_code::success };
			}
		};

		template <typename Unit, typename Point>
		LINGO_CONSTEXPR11 typename utf8<Unit, Point>::unit_bits_type utf8<Unit, Point>::first_unit_prefix_markers[5];
		template <typename Unit, typename Point>
		LINGO_CONSTEXPR11 typename utf8<Unit, Point>::unit_bits_type utf8<Unit, Point>::first_unit_data_masks[5];
	}
}

#include <lingo/encoding/endian.hpp>
#include <lingo/encoding/join.hpp>

namespace lingo
{
	namespace encoding
	{
		template <typename Unit, typename Point>
		using utf8_se = join<swap_endian<Unit>, utf8<Unit, Point>>;

		#if LINGO_ARCHITECTURE_ENDIANNESS == LINGO_ARCHITECTURE_LITTLE_ENDIAN
		template <typename Unit, typename Point>
		using utf8_le = utf8<Unit, Point>;
		template <typename Unit, typename Point>
		using utf8_be = utf8_se<Unit, Point>;
		#else
		template <typename Unit, typename Point>
		using utf8_le = utf8_se<Unit, Point>;
		template <typename Unit, typename Point>
		using utf8_be = utf8<Unit, Point>;
		#endif
	}

}

#endif