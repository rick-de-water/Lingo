#include <catch/catch.hpp>

#include <lingo/encoding/endian.hpp>
#include <lingo/string.hpp>

#include "test_case.hpp"
#include "test_types.hpp"
#include "test_strings.hpp"

#include <limits>
#include <type_traits>

TEST_CASE("Endianness encoding swaps around the bytes of each unit")
{
	using platform_endian_utf16_string = lingo::utf16_string;
	using opposite_endian_utf16_string = typename std::conditional<lingo::platform::is_big_endian(), lingo::utf16_le_string, lingo::utf16_be_string>::type;
	using platform_endian_utf32_string = lingo::utf32_string;
	using opposite_endian_utf32_string = typename std::conditional<lingo::platform::is_big_endian(), lingo::utf32_le_string, lingo::utf32_be_string>::type;

	const platform_endian_utf16_string platform_utf16 = lingo::test::test_string<char16_t>::value;
	const platform_endian_utf32_string platform_utf32 = lingo::test::test_string<char32_t>::value;

	const opposite_endian_utf16_string opposite_utf16(platform_utf16);
	const opposite_endian_utf16_string opposite_utf32(platform_utf32);

	const platform_endian_utf16_string platform_again_utf16(opposite_utf16);
	const platform_endian_utf32_string platform_again_utf32(opposite_utf32);

	REQUIRE(platform_utf16 == platform_again_utf16);
	REQUIRE(platform_utf32 == platform_again_utf32);

	bool boop = true;
}