#include <catch/catch.hpp>

#include <lingo/string.hpp>

#include <test_case.hpp>

LINGO_TEST_CASE("string has the correct types")
{
	LINGO_TEST_TYPEDEFS;

	// Typedefs
	REQUIRE(std::is_same<encoding_type, typename string::encoding_type>::value);
	REQUIRE(std::is_same<page_type, typename string::page_type>::value);

	REQUIRE(std::is_same<unit_type, typename string::value_type>::value);
	REQUIRE(std::is_same<unit_type&, typename string::reference>::value);
	REQUIRE(std::is_same<const unit_type&, typename string::const_reference>::value);
	REQUIRE(std::is_same<unit_type*, typename string::pointer>::value);
	REQUIRE(std::is_same<const unit_type*, typename string::const_pointer>::value);

	REQUIRE(std::is_same<size_type, typename string::size_type>::value);
	REQUIRE(std::is_same<difference_type, typename string::difference_type>::value);
}

LINGO_TEST_CASE("A default constructed string is an empty null terminated string")
{
	LINGO_TEST_TYPEDEFS;

	const string str;
	REQUIRE(str.size() == 0);
	REQUIRE(str.data()[0] == unit_type{});
}

LINGO_TEST_CASE("string can be constructed from a cstring")
{
	LINGO_TEST_TYPEDEFS;

	for (auto& test_cstring : lingo::test::generate_test_strings<encoding_type>())
	{
		const string test_string1(test_cstring.data);
		REQUIRE(test_string1.size() == test_cstring.size);
		REQUIRE(test_string1.data() != test_cstring.data);

		for (size_t i = 0; i < test_string1.size(); ++i)
		{
			REQUIRE(test_string1.data()[i] == test_cstring.data[i]);
		}
		REQUIRE(test_string1.data()[test_string1.size()] == unit_type{});


		const string test_string2(test_cstring.data, test_cstring.size);
		REQUIRE(test_string2.size() == test_cstring.size);
		REQUIRE(test_string2.data() != test_cstring.data);
		REQUIRE(test_string2.data()[test_string2.size()] == unit_type{});

		for (size_t i = 0; i < test_string2.size(); ++i)
		{
			REQUIRE(test_string2.data()[i] == test_cstring.data[i]);
		}
		REQUIRE(test_string2.data()[test_string2.size()] == unit_type{});


		const string test_string3(test_cstring.data, test_cstring.size / 2);
		REQUIRE(test_string3.size() == test_cstring.size / 2);
		REQUIRE(test_string3.data() != test_cstring.data);
		REQUIRE(test_string3.data()[test_string3.size()] == unit_type{});

		for (size_t i = 0; i < test_string3.size(); ++i)
		{
			REQUIRE(test_string3.data()[i] == test_cstring.data[i]);
		}
		REQUIRE(test_string3.data()[test_string3.size()] == unit_type{});
	}
}
