#include <catch/catch.hpp>

#include <lingo/strlen.hpp>

#include <test_strings.hpp>

namespace
{
	template <typename UnitT, size_t N>
	LINGO_CONSTEXPR17 std::array<size_t, N> calculate_lengths(const std::array<lingo::test::cstring<UnitT>, N>& test_strings)
	{
		std::array<size_t, N> lengths = {};

		for (size_t i = 0; i < test_strings.size(); ++i)
		{
			lengths[i] = lingo::strlen(test_strings[i].string);
		}

		return lengths;
	}
}

TEMPLATE_LIST_TEST_CASE("strlen has the correct exception specification", "", lingo::test::unit_types)
{
	REQUIRE(noexcept(lingo::strlen<TestType>(std::declval<const TestType*>())));
}

TEMPLATE_LIST_TEST_CASE("strlen_s has the correct exception specification", "", lingo::test::unit_types)
{
	REQUIRE(noexcept(lingo::strlen_s<TestType>(std::declval<const TestType*>(), std::declval<size_t>())));
}

TEMPLATE_LIST_TEST_CASE("strlen can find the char count of a c string", "", lingo::test::unit_types)
{
	LINGO_CONSTEXPR11 lingo::test::cstring_generator<TestType> generator;
	LINGO_CONSTEXPR11 auto test_strings = generator.generate();
	LINGO_CONSTEXPR17 auto test_string_lengths = calculate_lengths(test_strings);

	for (size_t i = 0; i < test_strings.size(); ++i)
	{
		REQUIRE(test_string_lengths[i] == test_strings[i].length);
	}
}

TEMPLATE_LIST_TEST_CASE("strlen_s can find the char count of a c string", "", lingo::test::unit_types)
{
	LINGO_CONSTEXPR11 lingo::test::cstring_generator<TestType> generator;
	LINGO_CONSTEXPR11 auto test_strings = generator.generate();

	for (size_t j = 0; j < 256; ++j)
	{
		for (size_t i = 0; i < test_strings.size(); ++i)
		{
			REQUIRE(lingo::strlen_s(static_cast<TestType*>(nullptr), j) == 0);
			REQUIRE(lingo::strlen_s(test_strings[i].string, j) == std::min<size_t>(j, test_strings[i].length));
		}
	}
}