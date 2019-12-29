#include <catch/catch.hpp>

#include <lingo/encoding/utf16.hpp>

#include "test_case.hpp"
#include "test_types.hpp"

#include <limits>

LINGO_UNIT_LEAST_16_TEST_CASE("utf16 defines the size of all points between 0 and 0x110000")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	const difference_type from = std::is_signed<point_type>::value ? -10 : 0;
	const difference_type to = std::numeric_limits<point_type>::digits > 21 ? 0x10FFFF + 10 : 0x10FFFF;

	for (difference_type i = from; i <= to; ++i)
	{
		size_type expected_size;
		if (i < 0)
		{
			expected_size = 0;
		}
		else if (i < 0xD800)
		{
			expected_size = 1;
		}
		else if (i < 0xE000)
		{
			expected_size = 0;
		}
		else if (i < 0x10000)
		{
			expected_size = 1;
		}
		else if (i < 0x110000)
		{
			expected_size = 2;
		}
		else
		{
			expected_size = 0;
		}

		const point_type point = point_type(i);
		const size_type point_size = lingo::encoding::utf16<unit_type, point_type>::point_size(point);

		REQUIRE(point_size == expected_size);
	}
}

LINGO_UNIT_LEAST_16_TEST_CASE("utf16 can get the size of a point from the first unit")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	const difference_type from = std::is_signed<unit_type>::value ? -10 : 0;
	const difference_type to = std::numeric_limits<unit_type>::digits > 16 ? 0xFFFF + 10 : 0xFFFF;

	for (difference_type i = from; i <= to; ++i)
	{
		size_type expected_size;
		if (i < 0)
		{
			expected_size = 0;
		}
		else if (i < 0xD800)
		{
			expected_size = 1;
		}
		else if (i < 0xDC00)
		{
			expected_size = 2;
		}
		else if (i < 0xE000)
		{
			expected_size = 0;
		}
		else if (i < 0x10000)
		{
			expected_size = 1;
		}
		else
		{
			expected_size = 0;
		}

		const unit_type unit = unit_type(i);
		const size_type unit_size = lingo::encoding::utf16<unit_type, point_type>::unit_size(unit);

		REQUIRE(unit_size == expected_size);
	}
}