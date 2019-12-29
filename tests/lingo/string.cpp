#include <catch/catch.hpp>

#include <lingo/string.hpp>

#include <lingo/encoding/point_iterator.hpp>

#include "test_case.hpp"
#include "test_strings.hpp"
#include "test_types.hpp"

#include <tuple>

TEST_CASE("string has the correct types")
{
	// Typedefs
	REQUIRE(std::is_same<lingo::encoding::none<char, char>, typename lingo::ascii_string::encoding_type>::value);
	REQUIRE(std::is_same<lingo::page::ascii, typename lingo::ascii_string::page_type>::value);

	REQUIRE(std::is_same<char, typename lingo::ascii_string::value_type>::value);
	REQUIRE(std::is_same<char&, typename lingo::ascii_string::reference>::value);
	REQUIRE(std::is_same<const char&, typename lingo::ascii_string::const_reference>::value);
	REQUIRE(std::is_same<char*, typename lingo::ascii_string::pointer>::value);
	REQUIRE(std::is_same<const char*, typename lingo::ascii_string::const_pointer>::value);

	REQUIRE(std::is_same<std::size_t, typename lingo::ascii_string::size_type>::value);
	REQUIRE(std::is_same<std::ptrdiff_t, typename lingo::ascii_string::difference_type>::value);
}

LINGO_UNIT_TEST_CASE("A default constructed string is an empty null terminated string")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	const string_type str;
	REQUIRE(str.size() == 0);
	REQUIRE(str.data()[0] == typename string_type::value_type{});
}

LINGO_UNIT_TEST_CASE("string can be constructed from a character array")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	const string_type test_string = lingo::test::test_string<unit_type>::value;

	const size_t size = (sizeof(lingo::test::test_string<unit_type>::value) / sizeof(lingo::test::test_string<unit_type>::value[0])) - 1;
	REQUIRE(test_string.size() == size);

	for (size_t i = 0; i < size; ++i)
	{
		CAPTURE(i);
		REQUIRE(test_string[i] == lingo::test::test_string<unit_type>::value[i]);
	}
}

LINGO_UNIT_TEST_CASE("string can be constructor from a code point and a count")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	const string_type source_string = lingo::test::test_string<unit_type>::value;
	point_iterator_type source_iterator(source_string);
	for (char32_t source_point : source_iterator)
	{
		for (std::size_t i = 0; i < (sizeof(string_type) / sizeof(unit_type)) * 2; ++i)
		{
			const string_type destination_string(i, source_point);
			std::size_t point_count = 0;
			point_iterator_type destination_iterator(destination_string);
			for (char32_t c : destination_iterator)
			{
				REQUIRE(c == source_point);
				++point_count;
			}

			REQUIRE(point_count == i);
		}
	}
}

LINGO_UNIT_TEST_CASE("string can be implicitly constructed from a string_view with the same page and encoding")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	const string_view_type test_string_view = string_view_type(lingo::test::test_string<unit_type>::value);
	const string_type test_string = test_string_view;

	REQUIRE(test_string.size() == test_string_view.size());

	for (size_t i = 0; i < test_string_view.size(); ++i)
	{
		CAPTURE(i);
		REQUIRE(test_string[i] == test_string_view[i]);
	}
}

TEMPLATE_LIST_TEST_CASE("string can be explicitly constructed from a string_view with a different page or encoding", "", lingo::test::unit_type_matrix)
{
	using source_unit_type = typename std::decay<decltype(std::get<0>(std::declval<TestType>()))>::type;
	using destination_unit_type = typename std::decay<decltype(std::get<1>(std::declval<TestType>()))>::type;

	using source_encoding_type = lingo::encoding::cstring_default_encoding_t<source_unit_type>;
	using destination_encoding_type = lingo::encoding::cstring_default_encoding_t<destination_unit_type>;

	using source_page_type = lingo::page::cstring_default_page_t<source_unit_type>;
	using destination_page_type = lingo::page::cstring_default_page_t<destination_unit_type>;

	using source_point_iterator_type = lingo::encoding::point_iterator<source_encoding_type>;
	using destination_point_iterator_type = lingo::encoding::point_iterator<destination_encoding_type>;

	using destination_string_type = lingo::basic_string<destination_encoding_type, destination_page_type>;
	using source_string_view_type = lingo::basic_string_view<source_encoding_type, source_page_type>;

	const source_string_view_type source_string_view(lingo::test::test_string<source_unit_type>::value);
	const destination_string_type destination_string(source_string_view);

	source_point_iterator_type source_iterator(source_string_view);
	destination_point_iterator_type destination_iterator(destination_string);

	while (source_iterator != source_point_iterator_type() && destination_iterator != destination_point_iterator_type())
	{
		REQUIRE(*source_iterator == *destination_iterator);

		++source_iterator;
		++destination_iterator;
	}

	REQUIRE(source_iterator == source_point_iterator_type());
	REQUIRE(destination_iterator == destination_point_iterator_type());
}