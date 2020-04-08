#include <catch/catch.hpp>

#include <lingo/page/ascii.hpp>
#include <lingo/page/unicode.hpp>

#include <lingo/page/intermediate.hpp>
#include <lingo/page/point_mapper.hpp>

#include <limits>
#include <type_traits>

TEST_CASE("ascii can be mapped to and from unicode")
{
	REQUIRE(std::is_same<typename lingo::page::intermediate<lingo::page::ascii, lingo::page::unicode_default>::type, lingo::page::unicode_default>::value);
	REQUIRE(std::is_same<typename lingo::page::intermediate<lingo::page::unicode_default, lingo::page::ascii>::type, lingo::page::unicode_default>::value);

	using source_page_type = lingo::page::ascii;
	using destination_page_type = lingo::page::unicode_default;

	using source_point_type = typename source_page_type::point_type;
	using destination_point_type = typename destination_page_type::point_type;

	using source_point_mapper = lingo::page::point_mapper<source_page_type, destination_page_type>;
	using destination_point_mapper = lingo::page::point_mapper<destination_page_type, source_page_type>;

	// Valid mappings
	for (std::ptrdiff_t i = 0; i < 128; ++i)
	{
		const source_point_type source_point = static_cast<source_point_type>(i);
		const destination_point_type destination_point = static_cast<destination_point_type>(i);

		const auto destination_result = source_page_type::map_to<destination_page_type>(source_point);
		REQUIRE(destination_result.error == lingo::error::error_code::success);
		REQUIRE(destination_result.point == destination_point);

		const auto source_result = source_page_type::map_from<destination_page_type>(destination_point);
		REQUIRE(source_result.error == lingo::error::error_code::success);
		REQUIRE(source_result.point == source_point);

		const auto mapped_destination_result = source_point_mapper::map(source_point);
		REQUIRE(mapped_destination_result.error == lingo::error::error_code::success);
		REQUIRE(mapped_destination_result.point == destination_point);

		const auto mapped_source_result = destination_point_mapper::map(destination_point);
		REQUIRE(mapped_source_result.error == lingo::error::error_code::success);
		REQUIRE(mapped_source_result.point == source_point);
	}

	// Invalid positive mappings
	for (std::ptrdiff_t i = 128; i < std::numeric_limits<char>::max(); ++i)
	{
		const source_point_type source_point = static_cast<source_point_type>(i);
		const destination_point_type destination_point = static_cast<destination_point_type>(i);

		const auto destination_result = source_page_type::map_to<destination_page_type>(source_point);
		REQUIRE(destination_result.error == lingo::error::error_code::no_mapping);

		const auto source_result = source_page_type::map_from<destination_page_type>(destination_point);
		REQUIRE(source_result.error == lingo::error::error_code::no_mapping);

		const auto mapped_destination_result = source_point_mapper::map(source_point);
		REQUIRE(mapped_destination_result.error == lingo::error::error_code::no_mapping);

		const auto mapped_source_result = destination_point_mapper::map(destination_point);
		REQUIRE(mapped_source_result.error == lingo::error::error_code::no_mapping);
	}

	// Invalid negative mappings
	for (std::ptrdiff_t i = std::numeric_limits<char>::min(); i < 0; ++i)
	{
		const source_point_type source_point = static_cast<source_point_type>(i);
		const destination_point_type destination_point = static_cast<destination_point_type>(i);

		const auto destination_result = source_page_type::map_to<destination_page_type>(source_point);
		REQUIRE(destination_result.error == lingo::error::error_code::no_mapping);

		const auto source_result = source_page_type::map_from<destination_page_type>(destination_point);
		REQUIRE(source_result.error == lingo::error::error_code::no_mapping);

		const auto mapped_destination_result = source_point_mapper::map(source_point);
		REQUIRE(mapped_destination_result.error == lingo::error::error_code::no_mapping);

		const auto mapped_source_result = destination_point_mapper::map(destination_point);
		REQUIRE(mapped_source_result.error == lingo::error::error_code::no_mapping);
	}
}