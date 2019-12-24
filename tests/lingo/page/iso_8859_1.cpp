#include <catch/catch.hpp>

#include <lingo/page/iso_8859_1.hpp>
#include <lingo/page/unicode.hpp>

#include <lingo/page/intermediate.hpp>
#include <lingo/page/point_mapper.hpp>

#include <limits>
#include <type_traits>

TEST_CASE("iso_8859_1 can be mapped to and from unicode")
{
	REQUIRE(std::is_same<typename lingo::page::intermediate<lingo::page::iso_8859_1, lingo::page::unicode>::type, lingo::page::unicode>::value);
	REQUIRE(std::is_same<typename lingo::page::intermediate<lingo::page::unicode, lingo::page::iso_8859_1>::type, lingo::page::unicode>::value);

	using source_page_type = lingo::page::iso_8859_1;
	using destination_page_type = lingo::page::unicode;

	using source_point_type = typename source_page_type::point_type;
	using destination_point_type = typename destination_page_type::point_type;

	using source_point_mapper = lingo::page::point_mapper<source_page_type, destination_page_type>;
	using destination_point_mapper = lingo::page::point_mapper<destination_page_type, source_page_type>;

	// Valid mappings
	for (std::size_t i = 0; i < 256; ++i)
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
}