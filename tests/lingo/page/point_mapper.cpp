#include <catch/catch.hpp>

#include <test_types.hpp>

#include <lingo/page/point_mapper.hpp>

#include <lingo/page/ascii.hpp>
#include <lingo/page/unicode.hpp>

#include <type_traits>

#define LINGO_TEST_LITERAL_ASCII(...) \
	__VA_ARGS__##"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F" \
	__VA_ARGS__##"\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F" \
	__VA_ARGS__##" !\"#$%&'()*+,-./" \
	__VA_ARGS__##"0123456789:;<=>?" \
	__VA_ARGS__##"@ABCDEFGHIJKLMNO" \
	__VA_ARGS__##"PQRSTUVWXYZ[\\]^_" \
	__VA_ARGS__##"`abcdefghijklmmo" \
	__VA_ARGS__##"pqrstuvwxyz{|}~\x7F"

namespace
{
	template <typename Page>
	struct test_string;

	template <>
	struct test_string<lingo::page::ascii>
	{
		static LINGO_CONSTEXPR11 const lingo::page::ascii::point_type value[] =
			LINGO_TEST_LITERAL_ASCII()
			"\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80";

		static LINGO_CONSTEXPR11 lingo::page::ascii::point_type invalid_point = '\x80';
	};

	template <>
	struct test_string<lingo::page::unicode>
	{
		static LINGO_CONSTEXPR11 const lingo::page::unicode::point_type value[] =
			LINGO_TEST_LITERAL_ASCII(U)
			U"ÀÁÂÃÄÈÉÊËÌÍÎÏÒÓÔÕÖÙÚÛÜ";

		static LINGO_CONSTEXPR11 lingo::page::unicode::point_type invalid_point = U'\xFFFF';
	};


	LINGO_CONSTEXPR11 lingo::page::ascii::point_type test_string<lingo::page::ascii>::value[];
	LINGO_CONSTEXPR11 lingo::page::ascii::point_type test_string<lingo::page::ascii>::invalid_point;
	LINGO_CONSTEXPR11 lingo::page::unicode::point_type test_string<lingo::page::unicode>::value[];
	LINGO_CONSTEXPR11 lingo::page::unicode::point_type test_string<lingo::page::unicode>::invalid_point;

	using test_pages = std::tuple<lingo::page::ascii, lingo::page::unicode>;
	using test_page_matrix = lingo::test::tuple_matrix_t<test_pages, test_pages>;
}

TEMPLATE_LIST_TEST_CASE("point_converter can convert points between code pages", "", test_page_matrix)
{
	using source_page = typename std::decay<decltype(std::get<0>(std::declval<TestType>()))>::type;
	using destination_page = typename std::decay<decltype(std::get<1>(std::declval<TestType>()))>::type;

	using source_point = typename source_page::point_type;
	using destination_point = typename destination_page::point_type;

	using point_mapper = lingo::page::point_mapper<source_page, destination_page>;

	const auto& source_string = test_string<source_page>::value;
	const auto& destination_string = test_string<destination_page>::value;

	LINGO_CONSTEXPR11 std::size_t source_length = sizeof(source_string) / sizeof(source_point);
	LINGO_CONSTEXPR11 std::size_t destination_length = sizeof(destination_string) / sizeof(destination_point);

	REQUIRE(source_length == destination_length);
	
	for (std::size_t i = 0; i < source_length; ++i)
	{
		const auto& source_point = source_string[i];
		const auto& destination_point = destination_string[i];

		const auto result = point_mapper::map(source_point);
		if (result.error == lingo::error::error_code::success)
		{
			REQUIRE(result.point == destination_point);
		}
		else
		{
			REQUIRE(result.point == test_string<destination_page>::invalid_point);
		}
	}
}