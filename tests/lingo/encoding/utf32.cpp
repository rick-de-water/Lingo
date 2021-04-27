#include <catch/catch.hpp>

#if LINGO_TEST_SPLIT
#include <lingo/encoding/utf32.hpp>
#else
#include <lingo/test/include_all.hpp>
#endif

#include <lingo/test/test_case.hpp>
#include <lingo/test/test_types.hpp>

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

		const auto result = lingo::encoding::utf32<unit_type, point_type>::encode_one(source, destination);

		if ((i >= 0 && i < 0xD800) || (i >= 0xE000 && i < 0x110000))
		{
			REQUIRE(result.error == lingo::error::error_code::success);
			REQUIRE(result.source.data() == source + 1);
			REQUIRE(result.source.size() == 0);
			REQUIRE(result.destination.data() == destination + 1);
			REQUIRE(result.destination.size() == 0);
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

		const auto result = lingo::encoding::utf32<unit_type, point_type>::decode_one(source, destination);

		if ((i >= 0 && i < 0xD800) || (i >= 0xE000 && i < 0x110000))
		{
			REQUIRE(result.error == lingo::error::error_code::success);
			REQUIRE(result.source.data() == source + 1);
			REQUIRE(result.source.size() == 0);
			REQUIRE(result.destination.data() == destination + 1);
			REQUIRE(result.destination.size() == 0);
			REQUIRE(destination[0] == static_cast<point_type>(i));
		}
		else
		{
			REQUIRE(result.error != lingo::error::error_code::success);
		}
	}
}