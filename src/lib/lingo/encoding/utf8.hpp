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
			using unit_type = Unit;
			using point_type = Point;

			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;

			static LINGO_CONSTEXPR11 size_type max_units = 4;
			static LINGO_CONSTEXPR11 size_type min_unit_bits = 8;
			static LINGO_CONSTEXPR11 size_type min_point_bits = 21;

			using bit_converter = internal::bit_converter<unit_type, min_unit_bits, point_type, min_point_bits>;
			using unit_bits_type = typename bit_converter::unit_bits_type;
			using point_bits_type = typename bit_converter::point_bits_type;

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

			static LINGO_CONSTEXPR14 size_type point_size(point_type point) noexcept
			{
				// Reject values below zero when there are enough magnitude bits
				LINGO_IF_CONSTEXPR(std::is_signed<point_type>::value && std::numeric_limits<point_type>::digits > min_point_bits)
				{
					if (point < 0)
					{
						return 0;
					}
				}

				// Convert to bits
				const point_bits_type point_bits = bit_converter::to_point_bits(point);

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

				// Reject values below zero when there are enough magnitude bits
				LINGO_IF_CONSTEXPR(std::is_signed<unit_type>::value && std::numeric_limits<unit_type>::digits > min_unit_bits)
				{
					if (unit < 0)
					{
						return 0;
					}
				}

				// Convert to bits
				const unit_bits_type unit_bits = bit_converter::to_unit_bits(unit);

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

			static LINGO_CONSTEXPR14 encode_result encode_point(point_type point, unit_type* buffer, size_type buffer_size) noexcept
			{
				// Calculate the number of units needed to encode this point
				const size_type required_size = point_size(point);
				if (required_size == 0)
				{
					return { 0, error::error_code::invalid_point };
				}
				assert(required_size <= max_units);

				// Check if the buffer is large enough to fit the encoded point
				if (required_size > buffer_size)
				{
					return { 0, error::error_code::buffer_too_small };
				}

				// Encode the first unit
				point_bits_type point_bits = bit_converter::to_point_bits(point);
				const size_type last_index = required_size - 1;
				const unit_bits_type first_unit_bits = first_unit_prefix_markers[required_size] | static_cast<unit_bits_type>(point_bits >> (6 * last_index));
				buffer[0] = bit_converter::from_unit_bits(first_unit_bits);

				// Encode the subsequent units
				for (size_t i = 1; i < required_size; ++i)
				{
					const unit_bits_type continuation_unit_bits = continuation_unit_prefix_marker | static_cast<unit_bits_type>((point >> (6 * (last_index - i))) & continuation_unit_data_mask);
					buffer[i] = bit_converter::from_unit_bits(continuation_unit_bits);
				}

				// Return the result
				return { required_size, error::error_code::success };
			}

			static LINGO_CONSTEXPR14 decode_result<point_type> decode_point(const unit_type* buffer, size_type buffer_size) noexcept
			{
				// Check if there is at least one unit
				if (buffer_size == 0)
				{
					return { {}, 0, error::error_code::buffer_too_small };
				}

				// Calculate the size of the code point
				const std::size_t required_size = unit_size(*buffer);

				// Check if the first code unit is valid
				if (required_size == 0)
				{
					return { {}, 0, error::error_code::invalid_first_unit };
				}
				assert(required_size <= max_units);

				// Check if there are enough code units left for the code point
				if (buffer_size < required_size)
				{
					return { {}, required_size, error::error_code::buffer_too_small };
				}

				// Get the bits from the first unit
				const unit_bits_type first_unit_bits = bit_converter::to_unit_bits(buffer[0]);
				point_bits_type point_bits = first_unit_bits & first_unit_data_masks[required_size];

				// Get the bits from all the continuation bytes
				for (std::size_t i = 1; i < required_size; ++i)
				{
					// Every continuation byte adds 6 bits of information, so we shift the code point to the left by 6
					point_bits <<= 6;

					// Check if this is a valid continuation byte
					const unit_bits_type continuation_unit_bits = bit_converter::to_unit_bits(buffer[i]);
					if ((continuation_unit_bits & continuation_unit_prefix_mask) != continuation_unit_prefix_marker)
					{
						return { {}, required_size, error::error_code::invalid_subsequent_unit };
					}

					// Add the bits to the code point
					point_bits |= continuation_unit_bits & continuation_unit_data_mask;
				}

				// Return the result
				return { bit_converter::from_point_bits(point_bits), required_size, error::error_code::success };
			}
		};

		template <typename Unit, typename Point>
		LINGO_CONSTEXPR11 typename utf8<Unit, Point>::unit_bits_type utf8<Unit, Point>::first_unit_prefix_markers[5];
		template <typename Unit, typename Point>
		LINGO_CONSTEXPR11 typename utf8<Unit, Point>::unit_bits_type utf8<Unit, Point>::first_unit_data_masks[5];
	}
}

#endif