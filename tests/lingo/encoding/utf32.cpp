#include <catch/catch.hpp>

#include <lingo/encoding/utf32.hpp>

#include <lingo/test_case.hpp>
#include <lingo/test_types.hpp>

#include <limits>

LINGO_UNIT_LEAST_32_TEST_CASE("utf32 can encode points")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	const difference_type from = std::is_signed<point_type>::value ? -10 : 0;
	const difference_type to = std::numeric_limits<point_type>::digits > 21 ? 0x10FFFF + 10 : 0x10FFFF;

	for (difference_type i = from; i <= to; ++i)
	{
		const point_type source[1] = { static_cast<point_type>(i) };
		unit_type destination[1];

		const auto result = lingo::encoding::utf32<unit_type, point_type>::encode_point(source, destination);

		if ((i >= 0 && i < 0xD800) || (i >= 0xE000 && i < 0x110000))
		{
			REQUIRE(result.error == lingo::error::error_code::success);
			REQUIRE(result.source_read == 1);
			REQUIRE(result.destination_written == 1);
			REQUIRE(destination[0] == static_cast<unit_type>(i));
		}
		else
		{
			REQUIRE(result.error != lingo::error::error_code::success);
		}
	}
}

LINGO_UNIT_LEAST_32_TEST_CASE("utf32 can decode points")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	const difference_type from = std::is_signed<point_type>::value ? -10 : 0;
	const difference_type to = std::numeric_limits<point_type>::digits > 21 ? 0x10FFFF + 10 : 0x10FFFF;

	for (difference_type i = from; i <= to; ++i)
	{
		const unit_type source[1] = { static_cast<unit_type>(i) };
		point_type destination[1];

		const auto result = lingo::encoding::utf32<unit_type, point_type>::decode_point(source, destination);

		if ((i >= 0 && i < 0xD800) || (i >= 0xE000 && i < 0x110000))
		{
			REQUIRE(result.error == lingo::error::error_code::success);
			REQUIRE(result.source_read == 1);
			REQUIRE(result.destination_written == 1);
			REQUIRE(destination[0] == static_cast<point_type>(i));
		}
		else
		{
			REQUIRE(result.error != lingo::error::error_code::success);
		}
	}
}