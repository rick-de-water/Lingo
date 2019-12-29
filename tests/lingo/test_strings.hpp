#ifndef H_LINGO_TEST_TESTSTRINGS
#define H_LINGO_TEST_TESTSTRINGS

#include <lingo/constexpr.hpp>

#include <test_types.hpp>

#include <array>
#include <cstddef>
#include <type_traits>

#define LINGO_TEST_STRING(...) __VA_ARGS__##"" \
	"Lorem ipsum dolor sit amet" \
	"Лорем ипсум долор сит амет" \
	"Λορεμ ιπσθμ δολορ σιτ αμετ" \
	"լոռեմ իպսում դոլոռ սիթ ամեթ" \
	"ლორემ იფსუმ დოლორ სით ამეთ" \
	"सादगि माध्यम वर्तमान पडता ध्येय दिनांक" \
	"込後個自会能幸居真近立哲策後料鳴働校" \
	"万打のく機振クヨリ向元ゅ客日ふょはで前革む" \
	"누구든지 법률에 의하지 아니하고는" \
	"كما أم غينيا المنتصر" \
	"בה תנך ספרדית חרטומים" \
	"😀😁😂🤣😃😄😅😆😉😊😋😎😍😘" \
	"🧥👚👕👖👔👗👙👘👠👡👢👞👟🥾🥿" \
	"👶👧🧒👦👩🧑👨👵🧓👴👲👳‍♀️👳‍♂️🧕"\
	"👶🏿👦🏿👧🏿👨🏿👩🏿👱🏿‍♀️👱🏿👴🏿👵🏿👲🏿👳🏿‍♀️👳🏿👮🏿‍♀️👮🏿"\
	"🥥🥝🍅🍆🥑🥦🥒🥬🌶🌽🥕🥔🍠🥐"

namespace lingo
{
	namespace test
	{
		template <typename Unit>
		struct test_string;

		template <>
		struct test_string<char>
		{
			static LINGO_CONSTEXPR11 char value[] = LINGO_TEST_STRING();
		};
		LINGO_CONSTEXPR11 char test_string<char>::value[];

		template <>
		struct test_string<wchar_t>
		{
			static constexpr wchar_t value[] = LINGO_TEST_STRING(L);
		};
		LINGO_CONSTEXPR11 wchar_t test_string<wchar_t>::value[];

		#ifdef __cpp_char8_t
		template <>
		struct test_string<char8_t>
		{
			static constexpr char8_t value[] = LINGO_TEST_STRING(u8);
		};
		LINGO_CONSTEXPR11 char8_t test_string<char8_t>::value[];
		#endif

		template <>
		struct test_string<char16_t>
		{
			static constexpr char16_t value[] = LINGO_TEST_STRING(u);
		};
		LINGO_CONSTEXPR11 char16_t test_string<char16_t>::value[];

		template <>
		struct test_string<char32_t>
		{
			static constexpr char32_t value[] = LINGO_TEST_STRING(U);
		};
		LINGO_CONSTEXPR11 char32_t test_string<char32_t>::value[];
	}
}

#endif