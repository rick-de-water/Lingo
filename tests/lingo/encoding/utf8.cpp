#include <catch/catch.hpp>

#include <lingo/encoding/utf8.hpp>

#include <test_case.hpp>

#include <limits>

LINGO_UNIT_TEST_CASE("utf8 defines the size of all points between 0 and 0x110000")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	difference_type range;
	if (std::numeric_limits<point_type>::max() >= 0x110000)
	{
		range = 0x110000;
	}
	else
	{
		range = static_cast<std::ptrdiff_t>(std::numeric_limits<point_type>::max()) - static_cast<std::ptrdiff_t>(std::numeric_limits<point_type>::min());
	}

	for (difference_type i = 0; i <= range; ++i)
	{
		const point_type point = point_type(i);
		const size_type point_size = lingo::encoding::utf8<unit_type, point_type>::point_size(point);

		size_t actual_point;
		if (point < 0)
		{
			LINGO_CONSTEXPR11 std::ptrdiff_t min = static_cast<std::ptrdiff_t>(std::numeric_limits<point_type>::min());
			actual_point = static_cast<size_t>((static_cast<std::ptrdiff_t>(point) - min) - min);
		}
		else
		{
			actual_point = point;
		}

		if (actual_point < 0x80)
		{
			REQUIRE(point_size == 1);
		}
		else if (actual_point < 0x800)
		{
			REQUIRE(point_size == 2);
		}
		else if (actual_point < 0x10000)
		{
			REQUIRE(point_size == 3);
		}
		else if (actual_point < 0x110000)
		{
			REQUIRE(point_size == 4);
		}
		else
		{
			REQUIRE(point_size == 0);
		}
	}
}

LINGO_UNIT_TEST_CASE("utf8 can get the size of a point from the first unit")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	for (difference_type i = 0; i <= 255; ++i)
	{
		size_type expected_size;
		if ((i & 0b1000'0000) == 0)
		{
			expected_size = 1;
		}
		else if ((i & 0b1110'0000) == 0b1100'0000)
		{
			expected_size = 2;
		}
		else if ((i & 0b1111'0000) == 0b1110'0000)
		{
			expected_size = 3;
		}
		else if ((i & 0b1111'1000) == 0b1111'0000)
		{
			expected_size = 4;
		}
		else
		{
			expected_size = 0;
		}

		const unit_type unit = unit_type(i);
		const size_type size = lingo::encoding::utf8<unit_type, point_type>::unit_size(unit);

		REQUIRE(size == expected_size);
	}
}