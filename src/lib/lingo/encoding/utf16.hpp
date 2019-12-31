#ifndef H_LINGO_ENCODING_UTF16
#define H_LINGO_ENCODING_UTF16

#include <lingo/constexpr.hpp>

#include <lingo/encoding/result.hpp>
#include <lingo/encoding/internal/bit_converter.hpp>

#include <cassert>
#include <cstring>
#include <climits>
#include <limits>
#include <type_traits>

namespace lingo
{
	namespace encoding
	{
		template <typename Unit, typename Point>
		struct utf16
		{
			using unit_type = Unit;
			using point_type = Point;

			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;

			static LINGO_CONSTEXPR11 size_type max_units = 2;
			static LINGO_CONSTEXPR11 size_type min_unit_bits = 16;
			static LINGO_CONSTEXPR11 size_type min_point_bits = 21;

			using bit_converter = internal::bit_converter<unit_type, min_unit_bits, point_type, min_point_bits>;
			using unit_bits_type = typename bit_converter::unit_bits_type;
			using point_bits_type = typename bit_converter::point_bits_type;

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

				// Multi unit encoding
				if (point_bits > 0xFFFF)
				{
					return 2;
				}
				else
				{
					// Reject surrogates
					if ((point_bits & 0xF800) == 0xD800)
					{
						return 0;
					}

					// Single unit encoding
					return 1;
				}
			}

			static LINGO_CONSTEXPR14 size_type unit_size(unit_type unit) noexcept
			{
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

				// Reject values above 0xFFFF
				LINGO_IF_CONSTEXPR(sizeof(unit_bits_type) * CHAR_BIT > min_unit_bits)
				{
					if (unit_bits > 0xFFFF)
					{
						return 0;
					}
				}

				// Check for surrogate code unit
				if ((unit_bits & 0xF800) == 0xD800)
				{
					// High surrogate
					if ((unit_bits & 0x0400) == 0)
					{
						return 2;
					}
					// Reject low surrogate
					else
					{
						return 0;
					}
				}
				// No surrogate
				else
				{
					return 1;
				}
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

				point_bits_type point_bits = bit_converter::to_point_bits(point);
				// Encode single unit
				if (required_size == 1)
				{
					const unit_bits_type unit_bits = static_cast<unit_bits_type>(point_bits);
					buffer[0] = bit_converter::from_unit_bits(unit_bits);
					return { 1, error::error_code::success };
				}
				// Encode surrogate pair
				else
				{
					assert(required_size == 2);

					point_bits -= 0x10000;
					const unit_bits_type high_unit_bits = static_cast<unit_bits_type>((point_bits >> 10) + 0xD800);
					const unit_bits_type low_unit_bits = static_cast<unit_bits_type>((point_bits & 0x3FF) + 0xDC00);

					buffer[0] = bit_converter::from_unit_bits(high_unit_bits);
					buffer[1] = bit_converter::from_unit_bits(low_unit_bits);

					return { 2, error::error_code::success };
				}
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

				// Decode single unit
				const unit_bits_type high_unit_bits = bit_converter::to_unit_bits(buffer[0]);
				if (required_size == 1)
				{
					const point_bits_type point_bits = static_cast<point_bits_type>(high_unit_bits);
					return { bit_converter::from_point_bits(point_bits), 1, error::error_code::success };
				}
				// Decode surrogate pair
				else
				{
					assert(required_size == 2);
					const unit_bits_type low_unit_bits = bit_converter::to_unit_bits(buffer[1]);

					// Validate low surrogate
					if ((low_unit_bits & 0xFC00) != 0xDC00)
					{
						return { {}, 2, error::error_code::invalid_subsequent_unit };
					}

					point_bits_type point_bits = static_cast<point_bits_type>(high_unit_bits - 0xD800);
					point_bits <<= 10;
					point_bits |= static_cast<point_bits_type>(low_unit_bits - 0xDC00);
					point_bits += 0x10000;

					return { bit_converter::from_point_bits(point_bits), 2, error::error_code::success };
				}
			}
		};
	}
}

#endif