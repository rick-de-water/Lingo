#ifndef H_LINGO_TEST_TESTSTRINGS
#define H_LINGO_TEST_TESTSTRINGS

#include <lingo/constexpr.hpp>

#include <array>
#include <cstddef>
#include <type_traits>

#define TEST_CSTRING_DECAY(string) static_cast<const typename std::remove_extent<typename std::remove_reference<decltype(string)>::type>::type*>(string)

#define TEST_CSTRING0(...) __VA_ARGS__""
#define TEST_CSTRING1(...) __VA_ARGS__"a"
#define TEST_CSTRING2(...) __VA_ARGS__"fgdfg"
#define TEST_CSTRING3(...) __VA_ARGS__"ghfgnghbndfggdfgsdfg"
#define TEST_CSTRING4(...) __VA_ARGS__"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam risus massa, gravida ac mauris et, fermentum dapibus nisi. Nunc egestas urna a suscipit tincidunt. Quisque mattis quam ac turpis tempor hendrerit. Sed vel eleifend lacus. Donec eget imperdiet odio, et fermentum massa. Phasellus augue ante, auctor id imperdiet eu, bibendum id urna. Maecenas vel massa ac lorem lacinia mollis sit amet ac dui. Aenean mollis tellus nisl, eget finibus ligula dictum vitae."
#define TEST_CSTRING5(...) __VA_ARGS__"abcdefghijklmnopqrstuvwxyz\0a12342346dfgdffgdsfasfgdfgdfgdfgcggngg67553454342413dfdfghdffasdfdfhghszghfgh"

#define TEST_CSTRINGS(UnitT, ...) { \
	cstring<UnitT>{ TEST_CSTRING0(__VA_ARGS__), 0 }, \
	cstring<UnitT>{ TEST_CSTRING1(__VA_ARGS__), 1 }, \
	cstring<UnitT>{ TEST_CSTRING2(__VA_ARGS__), 5 }, \
	cstring<UnitT>{ TEST_CSTRING3(__VA_ARGS__), 20 }, \
	cstring<UnitT>{ TEST_CSTRING4(__VA_ARGS__), 463 }, \
	cstring<UnitT>{ TEST_CSTRING5(__VA_ARGS__), 26 }, \
}


namespace lingo
{
	namespace test
	{
		using unit_types = std::tuple<
			char,
			wchar_t,
			#if __cpp_char8_t
			char8_t,
			#endif
			char16_t,
			char32_t
			>;

		template <typename UnitT>
		struct cstring
		{
			const UnitT* string;
			size_t length;
		};
		
		template <typename UnitT> struct cstring_generator;

		constexpr size_t cstring_count = 6;
		template <typename UnitT> using cstring_array = std::array<cstring<UnitT>, cstring_count>;

		template <>
		struct cstring_generator<char>
		{
			LINGO_CONSTEXPR11 cstring_array<char> generate() const noexcept
			{
				return TEST_CSTRINGS(char);
			}
		};

		template <>
		struct cstring_generator<signed char>
		{
			LINGO_CONSTEXPR11 cstring_array<signed char> generate() const noexcept
			{
				return TEST_CSTRINGS(signed char, (signed char*));
			}
		};

		template <>
		struct cstring_generator<wchar_t>
		{
			LINGO_CONSTEXPR11 cstring_array<wchar_t> generate() const noexcept
			{
				return TEST_CSTRINGS(wchar_t, L);
			}
		};

		#ifdef __cpp_char8_t
		template <>
		struct cstring_generator<char8_t>
		{
			LINGO_CONSTEXPR11 cstring_array<char8_t> generate() const noexcept
			{
				return TEST_CSTRINGS(char8_t, u8);
			}
		};
		#endif

		template <>
		struct cstring_generator<char16_t>
		{
			LINGO_CONSTEXPR11 cstring_array<char16_t> generate() const noexcept
			{
				return TEST_CSTRINGS(char16_t, u);
			}
		};

		template <>
		struct cstring_generator<char32_t>
		{
			LINGO_CONSTEXPR11 cstring_array<char32_t> generate() const noexcept
			{
				return TEST_CSTRINGS(char32_t, U);
			}
		};
	}
}

#endif