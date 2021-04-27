#include <catch/catch.hpp>

#if LINGO_TEST_SPLIT
#include <lingo/null_terminated_string.hpp>
#else
#include <lingo/test/include_all.hpp>
#endif

#include <lingo/test/test_case.hpp>
#include <lingo/test/test_strings.hpp>
#include <lingo/test/test_types.hpp>

#include <tuple>

LINGO_UNIT_TEST_CASE("null_terminated_string can be created from sting objects")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	using null_terminated_string_type = lingo::basic_null_terminated_string<encoding_type, page_type, allocator_type>;

	const string_view_type terminated_string_view(lingo::test::test_string<unit_type>::value);
	const string_view_type non_terminated_string_view(lingo::test::test_string<unit_type>::value, lingo::test::test_string<unit_type>::size, false);
	const string_type string(lingo::test::test_string<unit_type>::value);

	null_terminated_string_type nts_terminated_string_view(terminated_string_view);
	null_terminated_string_type nts_non_terminated_string_view(non_terminated_string_view);
	null_terminated_string_type nts_string(string);

	REQUIRE(nts_terminated_string_view.data() == terminated_string_view.data());
	REQUIRE(nts_non_terminated_string_view.data() != non_terminated_string_view.data());
	REQUIRE(nts_string.c_str() == string.data());

	REQUIRE(nts_terminated_string_view.size() == terminated_string_view.size());
	REQUIRE(nts_non_terminated_string_view.size() == non_terminated_string_view.size());
	REQUIRE(nts_string.size() == string.size());

	REQUIRE(nts_terminated_string_view.data() == nts_terminated_string_view.c_str());
	REQUIRE(nts_non_terminated_string_view.data() == nts_non_terminated_string_view.c_str());
	REQUIRE(nts_string.data() == nts_string.c_str());

	REQUIRE(nts_terminated_string_view.view() == terminated_string_view);
	REQUIRE(nts_non_terminated_string_view.view() == non_terminated_string_view);
	REQUIRE(nts_string.view() == string);

	REQUIRE(nts_terminated_string_view.view() == terminated_string_view);
	REQUIRE(nts_non_terminated_string_view.view() == non_terminated_string_view);
	REQUIRE(nts_string.view() == string);
}

LINGO_UNIT_TEST_CASE("make_null_terminated can be used to create instances of null_terminated_string")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	const string_view_type terminated_string_view(lingo::test::test_string<unit_type>::value);
	const string_view_type non_terminated_string_view(lingo::test::test_string<unit_type>::value, lingo::test::test_string<unit_type>::size, false);
	const string_type string(lingo::test::test_string<unit_type>::value);

	auto nts_terminated_string_view = lingo::make_null_terminated(terminated_string_view);
	auto nts_non_terminated_string_view = lingo::make_null_terminated(non_terminated_string_view);
	auto nts_string = lingo::make_null_terminated(string);

	REQUIRE(nts_terminated_string_view.data() == terminated_string_view.data());
	REQUIRE(nts_non_terminated_string_view.data() != non_terminated_string_view.data());
	REQUIRE(nts_string.c_str() == string.data());

	REQUIRE(nts_terminated_string_view.view() == terminated_string_view);
	REQUIRE(nts_non_terminated_string_view.view() == non_terminated_string_view);
	REQUIRE(nts_string.view() == string);
}