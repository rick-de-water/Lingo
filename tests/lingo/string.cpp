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

	using source_encoding_type = lingo::encoding::execution_encoding_t<source_unit_type>;
	using destination_encoding_type = lingo::encoding::execution_encoding_t<destination_unit_type>;

	using source_page_type = lingo::page::execution_page_t<source_unit_type>;
	using destination_page_type = lingo::page::execution_page_t<destination_unit_type>;

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

namespace
{
	template <typename T, typename U, typename std::enable_if<!std::is_pointer<typename std::decay<T>::type>::value, int>::type = 0>
	void test_compare(const T& left, const U& right, int expected_result)
	{
		if (expected_result < 0)
		{
			REQUIRE(left.compare(right) < 0);
			REQUIRE_FALSE(left == right);
			REQUIRE(left != right);
			REQUIRE(left < right);
			REQUIRE_FALSE(left > right);
			REQUIRE(left <= right);
			REQUIRE_FALSE(left >= right);
		}
		else if (expected_result > 0)
		{
			REQUIRE(left.compare(right) > 0);
			REQUIRE_FALSE(left == right);
			REQUIRE(left != right);
			REQUIRE_FALSE(left < right);
			REQUIRE(left > right);
			REQUIRE_FALSE(left <= right);
			REQUIRE(left >= right);
		}
		else
		{
			REQUIRE(left.compare(right) == 0);
			REQUIRE(left == right);
			REQUIRE_FALSE(left != right);
			REQUIRE_FALSE(left < right);
			REQUIRE_FALSE(left > right);
			REQUIRE(left <= right);
			REQUIRE(left >= right);
		}
	}

	template <typename T, typename U, typename std::enable_if<std::is_pointer<typename std::decay<T>::type>::value, int>::type = 0>
	void test_compare(const T& left, const U& right, int expected_result)
	{
		if (expected_result < 0)
		{
			REQUIRE_FALSE(left == right);
			REQUIRE(left != right);
			REQUIRE(left < right);
			REQUIRE_FALSE(left > right);
			REQUIRE(left <= right);
			REQUIRE_FALSE(left >= right);
		}
		else if (expected_result > 0)
		{
			REQUIRE_FALSE(left == right);
			REQUIRE(left != right);
			REQUIRE_FALSE(left < right);
			REQUIRE(left > right);
			REQUIRE_FALSE(left <= right);
			REQUIRE(left >= right);
		}
		else
		{
			REQUIRE(left == right);
			REQUIRE_FALSE(left != right);
			REQUIRE_FALSE(left < right);
			REQUIRE_FALSE(left > right);
			REQUIRE(left <= right);
			REQUIRE(left >= right);
		}
	}
}

LINGO_UNIT_TEST_CASE("strings can be compared")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	LINGO_CONSTEXPR11 size_type test_string_count = 11;

	string_type test_strings[test_string_count];
	struct test_string_results_type
	{
		int compare_results[test_string_count];
	};

	const string_view_type source = lingo::test::test_string<unit_type>::value;
	const lingo::utility::span<const unit_type> source_span(source.data(), source.size());
	point_type first_point;
	const lingo::utility::span<point_type> destination_span(&first_point, 1);
	encoding_type::decode_one(source_span, destination_span);

	for (char32_t point : point_iterator_type(source))
	{
		test_string_results_type test_string_results[test_string_count];
		for (size_t i = 0; i < test_string_count; ++i)
		{
			// Always equal to self
			test_string_results[i].compare_results[i] = 0;
		}

		// string 0 is empty
		for (size_t i = 1; i < test_string_count; ++i)
		{
			// Always lower than other strings
			test_string_results[0].compare_results[i] = -1;
			test_string_results[i].compare_results[0] = 1;
		}

		// string 10 is the string iterated up till now
		test_strings[10].append(1, point);

		// string 1-9 are point +/- 1 repeated 1, 2 or 3 times
		for (size_type i = 1; i <= 9; ++i)
		{
			const point_type actual_point = i <= 3 ? point - 1 : i <= 6 ? point : point + 1;
			const size_type length = i <= 3 ? i : i <= 6 ? i - 3 : i - 6;

			test_strings[i] = string_type(length, actual_point);

			for (size_type j = 1; j <= 9; ++j)
			{
				test_string_results[i].compare_results[j] = 0 - 1 * (i < j) + 1 * (i > j);
			}

			test_string_results[i].compare_results[10] = 0 - 1 * (actual_point < first_point) + 1 * (actual_point > first_point) - 1 * (actual_point == first_point && length == 1 && test_strings[10].size() > 1) + 1 * (actual_point == first_point && length > 1);
			test_string_results[10].compare_results[i] = -test_string_results[i].compare_results[10];
		}
		
		for (size_type i = 0; i < test_string_count; ++i)
		{
			const string_type& left = test_strings[i];
			string_view_type left_view(left);

			for (size_type j = 0; j < test_string_count; ++j)
			{
				const string_type& right = test_strings[j];
				string_view_type right_view(right);
				const int expected_result = test_string_results[i].compare_results[j];

				test_compare(left, right, expected_result);
				test_compare(left_view, right, expected_result);
				test_compare(left, right_view, expected_result);
				test_compare(left_view, right_view, expected_result);

				test_compare(left, right.data(), expected_result);
				test_compare(left.data(), right, expected_result);
				test_compare(left_view, right.data(), expected_result);
				test_compare(left.data(), right_view, expected_result);
			}
		}
	}
}

LINGO_UNIT_TEST_CASE("A string can be concatenated to another string")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	struct different_allocator_type : public allocator_type {};
	using different_allocator_string_type = lingo::basic_string<encoding_type, page_type, different_allocator_type>;

	REQUIRE_FALSE(std::is_same<string_type, different_allocator_string_type>::value);

	const string_view_type source_string = lingo::test::test_string<unit_type>::value;
	string_type test_string = source_string;
	different_allocator_string_type different_allocator_test_string = lingo::test::test_string<unit_type>::value;

	REQUIRE(test_string == different_allocator_test_string);

	SECTION("n + n")
	{
		const auto free_result = test_string + test_string;
		test_string += test_string;

		REQUIRE(free_result.size() == source_string.size() * 2);
		REQUIRE(test_string.size() == source_string.size() * 2);
		REQUIRE(different_allocator_test_string.size() == source_string.size());

		REQUIRE(string_view_type(free_result.data(), source_string.size()) == source_string);
		REQUIRE(string_view_type(free_result.data() + source_string.size(), source_string.size()) == source_string);

		REQUIRE(string_view_type(test_string.data(), source_string.size()) == source_string);
		REQUIRE(string_view_type(test_string.data() + source_string.size(), source_string.size()) == source_string);
	}

	SECTION("n + d")
	{
		const auto free_result = test_string + different_allocator_test_string;
		test_string += different_allocator_test_string;

		REQUIRE(free_result.size() == source_string.size() * 2);
		REQUIRE(test_string.size() == source_string.size() * 2);
		REQUIRE(different_allocator_test_string.size() == source_string.size());

		REQUIRE(string_view_type(free_result.data(), source_string.size()) == source_string);
		REQUIRE(string_view_type(free_result.data() + source_string.size(), source_string.size()) == source_string);

		REQUIRE(string_view_type(test_string.data(), source_string.size()) == source_string);
		REQUIRE(string_view_type(test_string.data() + source_string.size(), source_string.size()) == source_string);
	}

	SECTION("d + n")
	{
		const auto free_result = different_allocator_test_string + test_string;
		different_allocator_test_string += test_string;

		REQUIRE(free_result.size() == source_string.size() * 2);
		REQUIRE(test_string.size() == source_string.size());
		REQUIRE(different_allocator_test_string.size() == source_string.size() * 2);

		REQUIRE(string_view_type(free_result.data(), source_string.size()) == source_string);
		REQUIRE(string_view_type(free_result.data() + source_string.size(), source_string.size()) == source_string);

		REQUIRE(string_view_type(different_allocator_test_string.data(), source_string.size()) == source_string);
		REQUIRE(string_view_type(different_allocator_test_string.data() + source_string.size(), source_string.size()) == source_string);
	}

	SECTION("d + d")
	{
		const auto free_result = different_allocator_test_string + different_allocator_test_string;
		different_allocator_test_string += different_allocator_test_string;

		REQUIRE(free_result.size() == source_string.size() * 2);
		REQUIRE(test_string.size() == source_string.size());
		REQUIRE(different_allocator_test_string.size() == source_string.size() * 2);

		REQUIRE(string_view_type(free_result.data(), source_string.size()) == source_string);
		REQUIRE(string_view_type(free_result.data() + source_string.size(), source_string.size()) == source_string);

		REQUIRE(string_view_type(different_allocator_test_string.data(), source_string.size()) == source_string);
		REQUIRE(string_view_type(different_allocator_test_string.data() + source_string.size(), source_string.size()) == source_string);
	}
}

LINGO_UNIT_TEST_CASE("A string_view can be concatenated to a string")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	const string_view_type source_string = lingo::test::test_string<unit_type>::value;
	string_type prefix_test_string = source_string;
	string_type suffix_test_string = source_string;

	prefix_test_string = source_string + prefix_test_string;
	suffix_test_string = suffix_test_string + source_string;

	REQUIRE(string_view_type(prefix_test_string.data(), source_string.size()) == source_string);
	REQUIRE(string_view_type(prefix_test_string.data() + source_string.size(), source_string.size()) == source_string);

	REQUIRE(string_view_type(suffix_test_string.data(), source_string.size()) == source_string);
	REQUIRE(string_view_type(suffix_test_string.data() + source_string.size(), source_string.size()) == source_string);

	for (size_type i = 0; i < 10; ++i)
	{
		prefix_test_string += source_string;
		REQUIRE(prefix_test_string.size() == source_string.size() * (i + 3));

		for (size_type j = 0; j < i + 3; ++j)
		{
			REQUIRE(string_view_type(prefix_test_string.data() + source_string.size() * j, source_string.size()) == source_string);
		}
	}
}

LINGO_UNIT_TEST_CASE("A string_view can be concatenated to a string_view")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	const string_view_type source_string = lingo::test::test_string<unit_type>::value;
	string_type test_string;

	test_string = source_string + source_string;
	REQUIRE(test_string.size() == source_string.size() * 2);
	REQUIRE(string_view_type(test_string.data(), source_string.size()) == source_string);
	REQUIRE(string_view_type(test_string.data() + source_string.size(), source_string.size()) == source_string);

	for (size_type i = 0; i < 10; ++i)
	{
		test_string += source_string;
		REQUIRE(test_string.size() == source_string.size() * (i + 3));
	}
}

LINGO_UNIT_TEST_CASE("A character can be concatenated to a string")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	const string_view_type source_string = lingo::test::test_string<unit_type>::value;
	string_type prefix_test_string = source_string;
	string_type suffix_test_string = source_string;
	string_type suffix_assign_test_string = source_string;

	std::vector<char32_t> expected_points;

	for (char32_t point : point_iterator_type(source_string))
	{
		prefix_test_string = point + prefix_test_string;
		suffix_test_string = suffix_test_string + point;
		suffix_assign_test_string += point;

		REQUIRE(prefix_test_string[prefix_test_string.size()] == unit_type{});
		REQUIRE(suffix_test_string[suffix_test_string.size()] == unit_type{});
		REQUIRE(suffix_assign_test_string[suffix_assign_test_string.size()] == unit_type{});

		expected_points.push_back(point);
		point_iterator_type prefix_iterator(prefix_test_string);
		point_iterator_type suffix_iterator(suffix_test_string);
		point_iterator_type suffix_assign_iterator(suffix_assign_test_string);

		for (auto it = expected_points.rbegin(); it != expected_points.rend(); ++it)
		{
			REQUIRE(*prefix_iterator == *it);

			++prefix_iterator;
		}

		for (char32_t expected_point : point_iterator_type(source_string))
		{
			REQUIRE(*prefix_iterator == expected_point);
			REQUIRE(*suffix_iterator == expected_point);
			REQUIRE(*suffix_assign_iterator == expected_point);

			++prefix_iterator;
			++suffix_iterator;
			++suffix_assign_iterator;
		}

		for (auto it = expected_points.begin(); it != expected_points.end(); ++it)
		{
			REQUIRE(*suffix_iterator == *it);
			REQUIRE(*suffix_assign_iterator == *it);

			++suffix_iterator;
			++suffix_assign_iterator;
		}
	}
}

LINGO_UNIT_TEST_CASE("A cstring can be concatenated to a string")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	const string_view_type source_string = lingo::test::test_string<unit_type>::value;
	string_type prefix_test_string = source_string;
	string_type suffix_test_string = source_string;

	REQUIRE(source_string.null_terminated());
	prefix_test_string = source_string.data() + prefix_test_string;
	suffix_test_string = suffix_test_string + source_string.data();

	REQUIRE(string_view_type(prefix_test_string.data(), source_string.size()) == source_string);
	REQUIRE(string_view_type(prefix_test_string.data() + source_string.size(), source_string.size()) == source_string);

	REQUIRE(string_view_type(suffix_test_string.data(), source_string.size()) == source_string);
	REQUIRE(string_view_type(suffix_test_string.data() + source_string.size(), source_string.size()) == source_string);

	for (size_type i = 0; i < 10; ++i)
	{
		prefix_test_string += source_string.data();
		REQUIRE(prefix_test_string.size() == source_string.size() * (i + 3));

		for (size_type j = 0; j < i + 3; ++j)
		{
			REQUIRE(string_view_type(prefix_test_string.data() + source_string.size() * j, source_string.size()) == source_string);
		}
	}
}

LINGO_UNIT_TEST_CASE("A cstring can be concatenated to a string_view")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	const string_view_type source_string = lingo::test::test_string<unit_type>::value;
	const string_view_type prefix_test_string = source_string;
	const string_view_type suffix_test_string = source_string;

	REQUIRE(source_string.null_terminated());
	const string_type prefix_test_string_result = source_string.data() + prefix_test_string;
	const string_type suffix_test_string_result = suffix_test_string + source_string.data();

	REQUIRE(string_view_type(prefix_test_string_result.data(), source_string.size()) == source_string);
	REQUIRE(string_view_type(prefix_test_string_result.data() + source_string.size(), source_string.size()) == source_string);

	REQUIRE(string_view_type(suffix_test_string_result.data(), source_string.size()) == source_string);
	REQUIRE(string_view_type(suffix_test_string_result.data() + source_string.size(), source_string.size()) == source_string);
}

LINGO_UNIT_TEST_CASE("string can be copied to an array")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	const unit_type* data = lingo::test::test_string<unit_type>::value;
	const size_type size = sizeof(lingo::test::test_string<unit_type>::value) / sizeof(lingo::test::test_string<unit_type>::value[0]) - 1;
	const string_type source(data, size);

	for (auto pos_it = point_iterator_type(source); pos_it != point_iterator_type(); ++pos_it)
	{
		const size_type pos = pos_it.read_ptr() - source.data();

		for (auto size_it = pos_it; size_it != point_iterator_type(); ++size_it)
		{
			const size_type buffer_size = (size_it.read_ptr() - source.data()) - pos;
			auto buffer = std::unique_ptr<unit_type[]>(new unit_type[buffer_size]);

			const size_type copied_count = source.copy(buffer.get(), buffer_size, pos);

			REQUIRE(copied_count == buffer_size);
			REQUIRE(string_view_type(buffer.get(), buffer_size, false) == source.substr(pos, buffer_size));
		}
	}

	REQUIRE_THROWS_AS(source.copy(nullptr, 0, size + 1), std::out_of_range);
}