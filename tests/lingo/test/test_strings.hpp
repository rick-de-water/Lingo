#ifndef H_LINGO_TEST_TESTSTRINGS
#define H_LINGO_TEST_TESTSTRINGS

#include <lingo/platform/constexpr.hpp>

#include <lingo/test/test_types.hpp>

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
			static LINGO_CONSTEXPR11 std::size_t size = (sizeof(value) / sizeof(value[0])) - 1;
		};
		template <typename _>
		LINGO_CONSTEXPR11 char test_string<char, _>::value[];
		template <typename _>
		LINGO_CONSTEXPR11 std::size_t test_string<char, _>::size;

		template <typename _>
		struct test_string<wchar_t, _>
		{
			static LINGO_CONSTEXPR11 wchar_t value[] = LINGO_TEST_STRING(L);
			static LINGO_CONSTEXPR11 std::size_t size = (sizeof(value) / sizeof(value[0])) - 1;
		};
		template <typename _>
		LINGO_CONSTEXPR11 wchar_t test_string<wchar_t, _>::value[];
		template <typename _>
		LINGO_CONSTEXPR11 std::size_t test_string<wchar_t, _>::size;

		#ifdef __cpp_char8_t
		template <typename _>
		struct test_string<char8_t, _>
		{
			static LINGO_CONSTEXPR11 char8_t value[] = LINGO_TEST_STRING(u8);
			static LINGO_CONSTEXPR11 std::size_t size = (sizeof(value) / sizeof(value[0])) - 1;
		};
		template <typename _>
		LINGO_CONSTEXPR11 char8_t test_string<char8_t, _>::value[];
		template <typename _>
		LINGO_CONSTEXPR11 std::size_t test_string<char8_t, _>::size;
		#endif

		template <typename _>
		struct test_string<char16_t, _>
		{
			static LINGO_CONSTEXPR11 char16_t value[] = LINGO_TEST_STRING(u);
			static LINGO_CONSTEXPR11 std::size_t size = (sizeof(value) / sizeof(value[0])) - 1;
		};
		template <typename _>
		LINGO_CONSTEXPR11 char16_t test_string<char16_t, _>::value[];
		template <typename _>
		LINGO_CONSTEXPR11 std::size_t test_string<char16_t, _>::size;

		template <typename _>
		struct test_string<char32_t, _>
		{
			static LINGO_CONSTEXPR11 char32_t value[] = LINGO_TEST_STRING(U);
			static LINGO_CONSTEXPR11 std::size_t size = (sizeof(value) / sizeof(value[0])) - 1;
		};
		template <typename _>
		LINGO_CONSTEXPR11 char32_t test_string<char32_t, _>::value[];
		template <typename _>
		LINGO_CONSTEXPR11 std::size_t test_string<char32_t, _>::size;
	}
}

#endif