#include <catch/catch.hpp>

#include <lingo/page/iso_8859_1.hpp>
#include <lingo/page/iso_8859_2.hpp>
#include <lingo/page/unicode.hpp>

#include <lingo/page/intermediate.hpp>
#include <lingo/page/point_mapper.hpp>

#include <fstream>
#include <iomanip>
#include <limits>
#include <map>
#include <sstream>
#include <tuple>
#include <type_traits>

namespace
{
	template <typename Page, std::size_t Part>
	struct page_description
	{
		using page_type = Page;
		static LINGO_CONSTEXPR11 std::size_t part_index = Part;
	};

	using test_pages = std::tuple<
		page_description<lingo::page::iso_8859_1, 1>,
		page_description<lingo::page::iso_8859_2, 2>
		>;
}

TEMPLATE_LIST_TEST_CASE("iso_8859_x can be mapped to and from unicode", "", test_pages)
{
	using iso_page_type = typename TestType::page_type;
	using unicode_page_type = lingo::page::unicode;

	using iso_point_type = typename iso_page_type::point_type;
	using unicode_point_type = typename unicode_page_type::point_type;

	using iso_unicode_point_mapper = lingo::page::point_mapper<iso_page_type, unicode_page_type>;
	using unicode_iso_point_mapper = lingo::page::point_mapper<unicode_page_type, iso_page_type>;

	LINGO_CONSTEXPR11 std::size_t part_index = TestType::part_index;

	std::stringstream file_name;
	file_name << LINGO_SPEC_DIRECTORY;
	file_name << "/unicode/MAPPINGS/ISO8859/8859-";
	file_name << part_index;
	file_name << ".TXT";
	std::ifstream mapping_file(file_name.str());
	REQUIRE(mapping_file.is_open());

	std::map<uint_least32_t, uint_least32_t> to_unicode;
	std::map<uint_least32_t, uint_least32_t> from_unicode;

	while (!mapping_file.eof())
	{
		if (mapping_file.peek() != '0')
		{
			mapping_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}

		uint_least32_t iso_point, unicode_point;
		mapping_file >> std::hex >> iso_point;
		mapping_file >> std::hex >> unicode_point;
		mapping_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		REQUIRE(mapping_file.good());

		to_unicode.emplace(iso_point, unicode_point);
		from_unicode.emplace(unicode_point, iso_point);
	}

	// To unicode
	for (std::size_t i = 0; i < iso_page_type::point_range; ++i)
	{
		const iso_point_type iso_point = static_cast<iso_point_type>(i);
		const auto unicode_result = iso_page_type::template map_to<unicode_page_type>(iso_point);
		const auto mapped_unicode_result = iso_unicode_point_mapper::map(iso_point);

		const auto it = to_unicode.find(iso_point);
		if (it != to_unicode.end())
		{
			REQUIRE(unicode_result.error == lingo::error::error_code::success);
			REQUIRE(unicode_result.point == static_cast<unicode_point_type>(it->second));

			REQUIRE(mapped_unicode_result.error == lingo::error::error_code::success);
			REQUIRE(mapped_unicode_result.point == static_cast<unicode_point_type>(it->second));
		}
		else
		{
			REQUIRE(unicode_result.error == lingo::error::error_code::no_mapping);
			REQUIRE(mapped_unicode_result.error == lingo::error::error_code::no_mapping);
		}
	}

	// From unicode
	for (std::size_t i = 0; i < unicode_page_type::point_range; ++i)
	{
		const unicode_point_type unicode_point = static_cast<unicode_point_type>(i);
		const auto iso_result = iso_page_type::template map_from<unicode_page_type>(unicode_point);
		const auto mapped_iso_result = unicode_iso_point_mapper::map(unicode_point);

		const auto it = from_unicode.find(unicode_point);
		if (it != from_unicode.end())
		{
			REQUIRE(iso_result.error == lingo::error::error_code::success);
			REQUIRE(iso_result.point == static_cast<iso_point_type>(it->second));

			REQUIRE(mapped_iso_result.error == lingo::error::error_code::success);
			REQUIRE(mapped_iso_result.point == static_cast<unicode_point_type>(it->second));
		}
		else
		{
			REQUIRE(iso_result.error == lingo::error::error_code::no_mapping);
			REQUIRE(mapped_iso_result.error == lingo::error::error_code::no_mapping);
		}
	}
}