#ifndef H_LINGO_ENCODING_UTF16
#define H_LINGO_ENCODING_UTF16

#include <lingo/constexpr.hpp>

#include <lingo/encoding/result.hpp>

#include <cassert>
#include <cstring>
#include <climits>
#include <limits>
#include <memory>
#include <type_traits>

namespace lingo
{
	namespace encoding
	{
		template <typename Unit, typename Point>
		struct utf16
		{
			public:
			using unit_type = Unit;
			using point_type = Point;

			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;

			static LINGO_CONSTEXPR11 size_type max_units = 2;

			private:
			using unit_bits_type = typename std::make_unsigned<unit_type>::type;
			using point_bits_type = typename std::make_unsigned<point_type>::type;

			static_assert(sizeof(unit_type) == sizeof(unit_bits_type), "unit_type and unit_bits_type are the same size");
			static_assert(sizeof(point_type) == sizeof(point_bits_type), "unit_type and unit_bits_type are the same size");

			static_assert(sizeof(unit_type) * CHAR_BIT >= 16, "A single unit must have at least 16 bits");
			static_assert(sizeof(point_type) * CHAR_BIT >= 21, "A single point must have at least 21 bits");

			static constexpr bool unit_has_16_bit_maginude = std::numeric_limits<unit_type>::digits >= 16;

			static LINGO_CONSTEXPR14 point_bits_type to_point_bits(point_type point)
			{
				// Memcpy when less than 21 of the point bits are part of the magnitude
				LINGO_IF_CONSTEXPR(std::numeric_limits<point_type>::digits < 21)
				{
					point_bits_type point_bits;
					std::memcpy(&point_bits, &point, sizeof(point_type));
					return point_bits;
				}
				// Cast point to bits
				else
				{
					assert(point >= 0);
					return static_cast<point_bits_type>(point);
				}
			}

			static LINGO_CONSTEXPR14 unit_bits_type to_unit_bits(unit_type unit)
			{
				// Memcpy when less than 16 of the unit bits are part of the magnitude
				LINGO_IF_CONSTEXPR(std::numeric_limits<unit_type>::digits < 16)
				{
					unit_bits_type unit_bits;
					std::memcpy(&unit_bits, &unit, sizeof(unit_type));
					return unit_bits;
				}
				// Cast unit to bits
				else
				{
					assert(unit >= 0);
					return static_cast<unit_bits_type>(unit);
				}
			}

			static LINGO_CONSTEXPR14 point_type from_point_bits(point_bits_type point_bits)
			{
				// Memcpy when less than 21 of the point bits are part of the magnitude
				LINGO_IF_CONSTEXPR(std::numeric_limits<point_type>::digits < 21)
				{
					point_type point;
					std::memcpy(&point, &point_bits, sizeof(point_type));
					return point;
				}
				// Cast bits to point
				else
				{
					return static_cast<point_type>(point_bits);
				}
			}

			static LINGO_CONSTEXPR14 unit_type from_unit_bits(unit_bits_type unit_bits)
			{
				// Memcpy when less than 16 of the unit bits are part of the magnitude
				LINGO_IF_CONSTEXPR(std::numeric_limits<unit_type>::digits < 16)
				{
					unit_type unit;
					std::memcpy(&unit, &unit_bits, sizeof(unit_type));
					return unit;
				}
				// Cast bits to unit
				else
				{
					return static_cast<unit_type>(unit_bits);
				}
			}

			public:
			static LINGO_CONSTEXPR14 size_type point_size(point_type point) noexcept
			{
				// Reject values below zero when there are enough magnitude bits
				LINGO_IF_CONSTEXPR(std::is_signed<point_type>::value && std::numeric_limits<point_type>::digits > 21)
				{
					if (point < 0)
					{
						return 0;
					}
				}

				// Convert to bits
				const point_bits_type point_bits = to_point_bits(point);

				// Reject values above 0x10FFFF
				LINGO_IF_CONSTEXPR(sizeof(point_bits_type) * CHAR_BIT > 21)
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
				LINGO_IF_CONSTEXPR(std::is_signed<unit_type>::value && std::numeric_limits<unit_type>::digits > 16)
				{
					if (unit < 0)
					{
						return 0;
					}
				}

				// Convert to bits
				const unit_bits_type unit_bits = to_unit_bits(unit);

				// Reject values above 0xFFFF
				LINGO_IF_CONSTEXPR(sizeof(unit_bits_type) * CHAR_BIT > 16)
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

				point_bits_type point_bits = to_point_bits(point);
				// Encode single unit
				if (required_size == 1)
				{
					const unit_bits_type unit_bits = static_cast<unit_bits_type>(point_bits);
					buffer[0] = from_unit_bits(unit_bits);
					return { 1, error::error_code::success };
				}
				// Encode surrogate pair
				else
				{
					assert(required_size == 2);

					point_bits -= 0x10000;
					const unit_bits_type high_unit_bits = static_cast<unit_bits_type>((point_bits >> 10) + 0xD800);
					const unit_bits_type low_unit_bits = static_cast<unit_bits_type>((point_bits & 0x3FF) + 0xDC00);

					buffer[0] = from_unit_bits(high_unit_bits);
					buffer[1] = from_unit_bits(low_unit_bits);

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
				const std::size_t code_point_units = unit_size(*buffer);

				// Check if the first code unit is valid
				if (code_point_units == 0)
				{
					return { {}, 0, error::error_code::invalid_first_unit };
				}
				assert(code_point_units <= max_units);

				// Check if there are enough code units left for the code point
				if (buffer_size < code_point_units)
				{
					return { {}, code_point_units, error::error_code::buffer_too_small };
				}

				// Decode single unit
				const unit_bits_type high_unit_bits = to_unit_bits(buffer[0]);
				if (code_point_units == 1)
				{
					const point_bits_type point_bits = static_cast<point_bits_type>(high_unit_bits);
					return { from_point_bits(point_bits), 1, error::error_code::success };
				}
				// Decode surrogate pair
				else
				{
					assert(code_point_units == 2);
					const unit_bits_type low_unit_bits = to_unit_bits(buffer[1]);

					// Validate low surrogate
					if ((low_unit_bits & 0xFC00) != 0xDC00)
					{
						return { {}, 2, error::error_code::invalid_subsequent_unit };
					}

					point_bits_type point_bits = static_cast<point_bits_type>(high_unit_bits - 0xD800);
					point_bits <<= 10;
					point_bits |= static_cast<point_bits_type>(low_unit_bits - 0xDC00);
					point_bits += 0x10000;

					return { from_point_bits(point_bits), 2, error::error_code::success };
				}
			}
		};
	}
}

#endif