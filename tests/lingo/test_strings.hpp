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
		template <typename Unit, typename = void>
		struct test_string;

		template <typename _>
		struct test_string<char, _>
		{
			static LINGO_CONSTEXPR11 char value[] = LINGO_TEST_STRING();
		};
		template <typename _>
		LINGO_CONSTEXPR11 char test_string<char, _>::value[];

		template <typename _>
		struct test_string<wchar_t, _>
		{
			static constexpr wchar_t value[] = LINGO_TEST_STRING(L);
		};
		template <typename _>
		LINGO_CONSTEXPR11 wchar_t test_string<wchar_t, _>::value[];

		#ifdef __cpp_char8_t
		template <typename _>
		struct test_string<char8_t, _>
		{
			static constexpr char8_t value[] = LINGO_TEST_STRING(u8);
		};
		template <typename _>
		LINGO_CONSTEXPR11 char8_t test_string<char8_t, _>::value[];
		#endif

		template <typename _>
		struct test_string<char16_t, _>
		{
			static constexpr char16_t value[] = LINGO_TEST_STRING(u);
		};
		template <typename _>
		LINGO_CONSTEXPR11 char16_t test_string<char16_t, _>::value[];

		template <typename _>
		struct test_string<char32_t, _>
		{
			static constexpr char32_t value[] = LINGO_TEST_STRING(U);
		};
		template <typename _>
		LINGO_CONSTEXPR11 char32_t test_string<char32_t, _>::value[];
	}
}

#endif