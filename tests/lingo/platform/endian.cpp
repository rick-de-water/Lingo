#include <catch/catch.hpp>

#if LINGO_TEST_SPLIT
#include <lingo/platform/endian.hpp>
#else
#include <lingo/test/include_all.hpp>
#endif

#include <cstring>
#include <limits>
#include <tuple>

namespace
{
	using test_types = std::tuple<
		bool,
		char,
		signed char,
		unsigned char,
		wchar_t,
		#ifdef _cpp_char8_t
		char8_t,
		#endif
		char16_t,
		char32_t,
		short,
		unsigned short,
		int,
		unsigned int,
		long,
		unsigned long,
		long long,
		unsigned long long
	>;
}

TEST_CASE("A platform is either big endian or little endian")
{
	REQUIRE(lingo::platform::is_big_endian() != lingo::platform::is_little_endian());
}

TEMPLATE_LIST_TEST_CASE("The endianess of all integer types can be swapped", "", test_types)
{
	using int_type = TestType;

	const int_type start = GENERATE(
		static_cast<int_type>(0),
		static_cast<int_type>(1),
		static_cast<int_type>(2),
		static_cast<int_type>(3),
		static_cast<int_type>(-1),
		static_cast<int_type>(-2),
		static_cast<int_type>(-3),
		std::numeric_limits<int_type>::min(),
		std::numeric_limits<int_type>::min() + 1,
		std::numeric_limits<int_type>::min() + 2,
		std::numeric_limits<int_type>::min() + 3,
		std::numeric_limits<int_type>::max(),
		std::numeric_limits<int_type>::max() - 1,
		std::numeric_limits<int_type>::max() - 2,
		std::numeric_limits<int_type>::max() - 3);

	const int_type swapped_once = lingo::platform::swap_endian(start);
	const int_type swapped_twice = lingo::platform::swap_endian(swapped_once);

	REQUIRE(start == swapped_twice);

	unsigned char start_bytes[sizeof(int_type)];
	unsigned char swapped_once_bytes[sizeof(int_type)];

	std::memcpy(&start_bytes, &start, sizeof(int_type));
	std::memcpy(&swapped_once_bytes, &swapped_once, sizeof(int_type));

	for (size_t i = 0; i < sizeof(int_type); ++i)
	{
		size_t j = sizeof(int_type) - (i + 1);
		REQUIRE(start_bytes[i] == swapped_once_bytes[j]);
	}
}