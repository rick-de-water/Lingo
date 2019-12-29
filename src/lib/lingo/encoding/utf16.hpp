#ifndef H_LINGO_ENCODING_UTF16
#define H_LINGO_ENCODING_UTF16

#include <lingo/constexpr.hpp>

#include <lingo/encoding/result.hpp>

#include <cassert>
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

			private:
			using unit_bits_type = typename std::make_unsigned<unit_type>::type;
			using point_bits_type = typename std::make_unsigned<point_type>::type;

			static_assert(sizeof(unit_type) == sizeof(unit_bits_type), "unit_type and unit_bits_type are the same size");
			static_assert(sizeof(point_type) == sizeof(point_bits_type), "unit_type and unit_bits_type are the same size");

			static_assert(sizeof(unit_type) * CHAR_BIT >= 16, "A single unit must have at least 16 bits");
			static_assert(sizeof(point_type) * CHAR_BIT >= 21, "A single point must have at least 21 bits");

			static constexpr bool unit_has_16_bit_maginude = std::numeric_limits<unit_type>::digits >= 16;

			public:
			static LINGO_CONSTEXPR11 size_type max_units = 2;

			static LINGO_CONSTEXPR14 size_type point_size(point_type point) noexcept
			{
				point_bits_type point_bits;

				// Memcpy when less than 21 of the point bits are part of the magnitude
				LINGO_IF_CONSTEXPR(std::numeric_limits<point_type>::digits < 21)
				{
					std::memcpy(&point_bits, &point, sizeof(point_type));
				}
				else
				{
					// Reject values below zero
					LINGO_IF_CONSTEXPR(std::is_signed<point_type>::value)
					{
						if (point < 0)
						{
							return 0;
						}
					}

					// We can now safely cast the point to an unsigned integer
					point_bits = static_cast<point_bits_type>(point);
				}

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
				unit_bits_type unit_bits;

				// Memcpy when less than 21 of the unit bits are part of the magnitude
				LINGO_IF_CONSTEXPR(std::numeric_limits<unit_type>::digits < 16)
				{
					std::memcpy(&unit_bits, &unit, sizeof(point_type));
				}
				else
				{
					// Reject values below zero
					LINGO_IF_CONSTEXPR(std::is_signed<unit_type>::value)
					{
						if (unit < 0)
						{
							return 0;
						}
					}

					// We can now safely cast the unit to an unsigned integer
					unit_bits = static_cast<unit_bits_type>(unit);
				}

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
				return {};
			}

			static LINGO_CONSTEXPR14 decode_result<point_type> decode_point(const unit_type* buffer, size_type buffer_size) noexcept
			{
				return {};
			}
		};
	}
}

#endif