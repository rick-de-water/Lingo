#ifndef H_LINGO_TEST_TESTSTRINGS
#define H_LINGO_TEST_TESTSTRINGS

#include <lingo/constexpr.hpp>

#include <test_types.hpp>

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

#define TEST_CSTRINGS(Unit, ...) { \
	cstring<Unit>{ TEST_CSTRING0(__VA_ARGS__), 0 }, \
	cstring<Unit>{ TEST_CSTRING1(__VA_ARGS__), 1 }, \
	cstring<Unit>{ TEST_CSTRING2(__VA_ARGS__), 5 }, \
	cstring<Unit>{ TEST_CSTRING3(__VA_ARGS__), 20 }, \
	cstring<Unit>{ TEST_CSTRING4(__VA_ARGS__), 463 }, \
	cstring<Unit>{ TEST_CSTRING5(__VA_ARGS__), 26 }, \
}


namespace lingo
{
	namespace test
	{
		struct base_test_string
		{
			const char32_t* data;
			std::size_t size;
		};

		LINGO_CONSTEXPR11 base_test_string base_test_strings[] =
		{
			{U"", 0},
			{U"a", 1},
			{U"fgdfg", 5},
			{U"ghfgnghbndfggdfgsdfg", 32},
			{U"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam risus massa, gravida ac mauris et, fermentum dapibus nisi. Nunc egestas urna a suscipit tincidunt. Quisque mattis quam ac turpis tempor hendrerit. Sed vel eleifend lacus. Donec eget imperdiet odio, et fermentum massa. Phasellus augue ante, auctor id imperdiet eu, bibendum id urna. Maecenas vel massa ac lorem lacinia mollis sit amet ac dui. Aenean mollis tellus nisl, eget finibus ligula dictum vitae.", 463},
			{U"abcdefghijklmnopqrstuvwxyz\0a12342346dfgdffgdsfasfgdfgdfgdfgcggngg67553454342413dfdfghdffasdfdfhghszghfgh", 26},
			{U"모든 국민은 종교의 자유를 가진다, 모든 국민은 직업선택의 자유를 가진다. 대법원장과 대법관이 ", 53},
			{U"दारी विकास मेंभटृ स्वतंत्र गुजरना समस्याओ सामूहिक पढाए देखने तकनीकी अविरोधता निर्माता उन्हे लिये शुरुआत उद्योग शारिरिक विषय होसके उनको एसलिये पासपाई एकत्रित समाजो ऎसाजीस शारिरिक पहोच शीघ्र समजते दर्शाता शारिरिक अर्थपुर्ण सहित कर्य बनाने शुरुआत सुचनाचलचित्र संपुर्ण स्वतंत्र ढांचा पहोच सहयोग कोहम नयेलिए वास्तविक भाषा शुरुआत निरपेक्ष सादगि भीयह दिशामे अनुकूल निर्माण नवंबर जैसे आधुनिक परिभाषित भेदनक्षमता बनाकर", 419},
			{U"製へーは程葉均がつそ選再ツサトチ愛視ぜび馬周ノユスフ繰載に一之職手安げどと際白づらお権端じび造59庁課偏啓87元ムノコル作棋稲那けい。点ん営転がスの同根ニヌヲメ界不93逸テコネ省63援び最生らしっぱ感択ヱテ発個転ハ売報こえゆけ雨常サユフ前9無る図見ニラユエ舎著負ユヒ主権り翻布益げ。", 141},
			{U"ლორემ იფსუმ დოლორ სით ამეთ, მეი ფოსსით ფაცილის აცცუსამუს უთ, ფაულო სცრიფთა ყუი უთ. ჰას ცუ ჰაბემუს ადოლესცენს, ეუ უსუ ცეთერო ინთელლეგებათ. ნიბჰ მეის ეამ თე, ეთ დოლორე ფეთენთიუმ დიგნისსიმ იუს. ეუ ნუმყუამ ვოლუმუს ფორენსიბუს სეა. ენიმ ჰომერო თე ჰის, ეუმ ან ელით ომნეს. ეთ უსუ ცოფიოსაე თაციმათეს, იდ ჰას მუციუს ცომმუნე ინციდერინთ.", 325},
		};

		/*template <typename Unit>
		struct cstring
		{
			const Unit* string;
			size_t length;
		};
		
		template <typename Unit> struct cstring_generator;

		constexpr size_t cstring_count = 6;
		template <typename Unit> using cstring_array = std::array<cstring<Unit>, cstring_count>;

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
		};*/
	}
}

#endif