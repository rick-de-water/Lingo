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

	const string_type str1;
	REQUIRE(str1.size() == 0);
	REQUIRE(str1.data()[0] == typename string_type::value_type{});

	const string_type str2(allocator_type{});
	REQUIRE(str2.size() == 0);
	REQUIRE(str2.data()[0] == typename string_type::value_type{});
}

LINGO_UNIT_TEST_CASE("A string can be copy constructed")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	const string_type test_string(lingo::test::test_string<unit_type>::value);

	const string_type test_string_copy1(test_string);
	const string_type test_string_copy2(test_string, allocator_type{});

	for (size_t i = 0; i < lingo::test::test_string<unit_type>::size; ++i)
	{
		REQUIRE(test_string.data()[i] == lingo::test::test_string<unit_type>::value[i]);
		REQUIRE(test_string_copy1.data()[i] == lingo::test::test_string<unit_type>::value[i]);
		REQUIRE(test_string_copy2.data()[i] == lingo::test::test_string<unit_type>::value[i]);
	}
}

LINGO_UNIT_TEST_CASE("A string can move constructed")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	string_type test_string1(lingo::test::test_string<unit_type>::value);
	string_type test_string2(lingo::test::test_string<unit_type>::value);

	const string_type test_string_moved1(std::move(test_string1));
	const string_type test_string_moved2(std::move(test_string2), allocator_type{});

	REQUIRE(test_string1.capacity() < sizeof(string_type) / sizeof(unit_type));
	REQUIRE(test_string2.capacity() < sizeof(string_type) / sizeof(unit_type));

	for (size_t i = 0; i < lingo::test::test_string<unit_type>::size; ++i)
	{
		REQUIRE(test_string_moved1.data()[i] == lingo::test::test_string<unit_type>::value[i]);
		REQUIRE(test_string_moved2.data()[i] == lingo::test::test_string<unit_type>::value[i]);
	}
}

LINGO_UNIT_TEST_CASE("A string can be copy assigned")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	const size_t point_count = GENERATE(range(0, 100));
	string_type test_string(point_count, lingo::test::test_string<unit_type>::value[0]);
	const string_type other_test_string(lingo::test::test_string<unit_type>::value);
	test_string = other_test_string;

	for (size_t i = 0; i < lingo::test::test_string<unit_type>::size; ++i)
	{
		REQUIRE(test_string.data()[i] == lingo::test::test_string<unit_type>::value[i]);
	}

	// Make sure copying self also works
	test_string = test_string;

	for (size_t i = 0; i < lingo::test::test_string<unit_type>::size; ++i)
	{
		REQUIRE(test_string.data()[i] == lingo::test::test_string<unit_type>::value[i]);
	}
}

LINGO_UNIT_TEST_CASE("A string can be move assigned")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	const size_t point_count = GENERATE(range(0, 100));
	string_type test_string(point_count, lingo::test::test_string<unit_type>::value[0]);
	string_type other_test_string(lingo::test::test_string<unit_type>::value);
	test_string = std::move(other_test_string);

	REQUIRE(other_test_string.capacity() < sizeof(string_type) / sizeof(unit_type));

	for (size_t i = 0; i < lingo::test::test_string<unit_type>::size; ++i)
	{
		REQUIRE(test_string.data()[i] == lingo::test::test_string<unit_type>::value[i]);
	}

	// Make sure moving self also works
	test_string = std::move(test_string);

	for (size_t i = 0; i < lingo::test::test_string<unit_type>::size; ++i)
	{
		REQUIRE(test_string.data()[i] == lingo::test::test_string<unit_type>::value[i]);
	}
}

LINGO_UNIT_TEST_CASE("string can be constructed from a character array")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	const string_type test_string = lingo::test::test_string<unit_type>::value;

	REQUIRE(test_string.size() == lingo::test::test_string<unit_type>::size);

	for (size_t i = 0; i < lingo::test::test_string<unit_type>::size; ++i)
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

LINGO_UNIT_TEST_CASE("A string can be copy constructed with a position offset")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	const size_type pos = GENERATE(range<size_type>(0, lingo::test::test_string<unit_type>::size));

	const string_type test_string(lingo::test::test_string<unit_type>::value);
	string_type test_string_copy;

	SECTION("Without alloctator")
	{
		string_type copy(test_string, pos);
		test_string_copy = std::move(copy);
	}
	SECTION("With allocator")
	{
		string_type copy(test_string, pos, allocator_type());
		test_string_copy = std::move(copy);
	}

	REQUIRE(test_string_copy.size() == test_string.size() - pos);

	for (size_t i = pos; i < test_string.size(); ++i)
	{
		REQUIRE(test_string_copy[i - pos] == test_string[i]);
	}
}

LINGO_UNIT_TEST_CASE("A string can be copy constructed with a position offset and a count")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	const size_type pos = GENERATE(range<size_type>(0, lingo::test::test_string<unit_type>::size));
	size_type count = std::min<size_type>(lingo::test::test_string<unit_type>::size - pos, GENERATE(0, 1, 2, 10, 100, lingo::test::test_string<unit_type>::size));

	const string_type test_string(lingo::test::test_string<unit_type>::value);
	string_type test_string_copy;

	SECTION("Without alloctator")
	{
		string_type copy(test_string, pos, count);
		test_string_copy = std::move(copy);
	}
	SECTION("With allocator")
	{
		string_type copy(test_string, pos, count, allocator_type());
		test_string_copy = std::move(copy);
	}
	SECTION("With npos")
	{
		string_type copy(test_string, pos, string_type::npos);
		test_string_copy = std::move(copy);
		count = test_string.size() - pos;
	}

	REQUIRE(test_string_copy.size() == count);

	for (size_t i = pos; i < count; ++i)
	{
		REQUIRE(test_string_copy[i - pos] == test_string[i]);
	}
}