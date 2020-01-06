#ifndef H_LINGO_ENCODING_INTERNAL_BIT_CONVERTER
#define H_LINGO_ENCODING_INTERNAL_BIT_CONVERTER

#include <lingo/platform/constexpr.hpp>

#include <cassert>
#include <climits>
#include <cstdint>
#include <cstring>
#include <type_traits>

namespace lingo
{
	namespace encoding
	{
		namespace internal
		{
			template <typename Unit, std::size_t MinUnitBits, typename Point, std::size_t MinPointBits>
			struct bit_converter
			{
				using unit_type = Unit;
				using point_type = Point;

				using size_type = std::size_t;
				using difference_type = std::ptrdiff_t;

				using unit_bits_type = typename std::make_unsigned<unit_type>::type;
				using point_bits_type = typename std::make_unsigned<point_type>::type;

				static LINGO_CONSTEXPR11 size_type min_unit_bits = MinUnitBits;
				static LINGO_CONSTEXPR11 size_type min_point_bits = MinPointBits;

				static_assert(sizeof(unit_type) == sizeof(unit_bits_type), "unit_type and unit_bits_type are the same size");
				static_assert(sizeof(point_type) == sizeof(point_bits_type), "unit_type and unit_bits_type are the same size");

				static_assert(sizeof(unit_type) * CHAR_BIT >= min_unit_bits, "unit_type has the minimum amount of required bits");
				static_assert(sizeof(point_type) * CHAR_BIT >= min_point_bits, "point_bits_type has the minimum amount of required bits");

				static LINGO_CONSTEXPR14 point_bits_type to_point_bits(point_type point)
				{
					// Memcpy when less than the minimum required amount of the bits are part of the magnitude
					LINGO_IF_CONSTEXPR(std::numeric_limits<point_type>::digits < min_point_bits)
					{
						point_bits_type point_bits{};
						std::memcpy(&point_bits, &point, sizeof(point_bits));
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
					// Memcpy when less than the minimum required amount of the bits are part of the magnitude
					LINGO_IF_CONSTEXPR(std::numeric_limits<unit_type>::digits < min_unit_bits)
					{
						unit_bits_type unit_bits{};
						std::memcpy(&unit_bits, &unit, sizeof(unit_bits));
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
					// Memcpy when less than the minimum required amount of the bits are part of the magnitude
					LINGO_IF_CONSTEXPR(std::numeric_limits<point_type>::digits < min_point_bits)
					{
						point_type point{};
						std::memcpy(&point, &point_bits, sizeof(point));
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
					// Memcpy when less than the minimum required amount of the bits are part of the magnitude
					LINGO_IF_CONSTEXPR(std::numeric_limits<unit_type>::digits < min_unit_bits)
					{
						unit_type unit{};
						std::memcpy(&unit, &unit_bits, sizeof(unit));
						return unit;
					}
					// Cast bits to unit
					else
					{
						return static_cast<unit_type>(unit_bits);
					}
				}
			};
		}
	}
}

#endif