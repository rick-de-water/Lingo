#include <catch/catch.hpp>

#include <lingo/encoding/base.hpp>
#include <lingo/encoding/join.hpp>

#include <lingo/platform/warnings.hpp>

#include <lingo/string.hpp>

#include <lingo/test_case.hpp>
#include <lingo/test_types.hpp>
#include <lingo/test_strings.hpp>

#include <limits>
#include <type_traits>

namespace
{
	LINGO_WARNINGS_PUSH_AND_DISABLE_CLANG(char-subscripts)
	template <typename T>
	void test_to_base_base64(typename T::unit_type u62, typename T::unit_type u63)
	{
		REQUIRE(T::table_to_base[0] == 'A');
		REQUIRE(T::table_to_base[1] == 'B');
		REQUIRE(T::table_to_base[2] == 'C');
		REQUIRE(T::table_to_base[3] == 'D');
		REQUIRE(T::table_to_base[4] == 'E');
		REQUIRE(T::table_to_base[5] == 'F');
		REQUIRE(T::table_to_base[6] == 'G');
		REQUIRE(T::table_to_base[7] == 'H');
		REQUIRE(T::table_to_base[8] == 'I');
		REQUIRE(T::table_to_base[9] == 'J');
		REQUIRE(T::table_to_base[10] == 'K');
		REQUIRE(T::table_to_base[11] == 'L');
		REQUIRE(T::table_to_base[12] == 'M');
		REQUIRE(T::table_to_base[13] == 'N');
		REQUIRE(T::table_to_base[14] == 'O');
		REQUIRE(T::table_to_base[15] == 'P');
		REQUIRE(T::table_to_base[16] == 'Q');
		REQUIRE(T::table_to_base[17] == 'R');
		REQUIRE(T::table_to_base[18] == 'S');
		REQUIRE(T::table_to_base[19] == 'T');
		REQUIRE(T::table_to_base[20] == 'U');
		REQUIRE(T::table_to_base[21] == 'V');
		REQUIRE(T::table_to_base[22] == 'W');
		REQUIRE(T::table_to_base[23] == 'X');
		REQUIRE(T::table_to_base[24] == 'Y');
		REQUIRE(T::table_to_base[25] == 'Z');

		REQUIRE(T::table_to_base[26] == 'a');
		REQUIRE(T::table_to_base[27] == 'b');
		REQUIRE(T::table_to_base[28] == 'c');
		REQUIRE(T::table_to_base[29] == 'd');
		REQUIRE(T::table_to_base[30] == 'e');
		REQUIRE(T::table_to_base[31] == 'f');
		REQUIRE(T::table_to_base[32] == 'g');
		REQUIRE(T::table_to_base[33] == 'h');
		REQUIRE(T::table_to_base[34] == 'i');
		REQUIRE(T::table_to_base[35] == 'j');
		REQUIRE(T::table_to_base[36] == 'k');
		REQUIRE(T::table_to_base[37] == 'l');
		REQUIRE(T::table_to_base[38] == 'm');
		REQUIRE(T::table_to_base[39] == 'n');
		REQUIRE(T::table_to_base[40] == 'o');
		REQUIRE(T::table_to_base[41] == 'p');
		REQUIRE(T::table_to_base[42] == 'q');
		REQUIRE(T::table_to_base[43] == 'r');
		REQUIRE(T::table_to_base[44] == 's');
		REQUIRE(T::table_to_base[45] == 't');
		REQUIRE(T::table_to_base[46] == 'u');
		REQUIRE(T::table_to_base[47] == 'v');
		REQUIRE(T::table_to_base[48] == 'w');
		REQUIRE(T::table_to_base[49] == 'x');
		REQUIRE(T::table_to_base[50] == 'y');
		REQUIRE(T::table_to_base[51] == 'z');

		REQUIRE(T::table_to_base[52] == '0');
		REQUIRE(T::table_to_base[53] == '1');
		REQUIRE(T::table_to_base[54] == '2');
		REQUIRE(T::table_to_base[55] == '3');
		REQUIRE(T::table_to_base[56] == '4');
		REQUIRE(T::table_to_base[57] == '5');
		REQUIRE(T::table_to_base[58] == '6');
		REQUIRE(T::table_to_base[59] == '7');
		REQUIRE(T::table_to_base[60] == '8');
		REQUIRE(T::table_to_base[61] == '9');

		REQUIRE(T::table_to_base[62] == u62);
		REQUIRE(T::table_to_base[63] == u63);
	}

	template <typename T>
	void test_from_base_base64(typename T::unit_type u62, typename T::unit_type u63)
	{
		REQUIRE(T::table_from_base['A'] == 0);
		REQUIRE(T::table_from_base['B'] == 1);
		REQUIRE(T::table_from_base['C'] == 2);
		REQUIRE(T::table_from_base['D'] == 3);
		REQUIRE(T::table_from_base['E'] == 4);
		REQUIRE(T::table_from_base['F'] == 5);
		REQUIRE(T::table_from_base['G'] == 6);
		REQUIRE(T::table_from_base['H'] == 7);
		REQUIRE(T::table_from_base['I'] == 8);
		REQUIRE(T::table_from_base['J'] == 9);
		REQUIRE(T::table_from_base['K'] == 10);
		REQUIRE(T::table_from_base['L'] == 11);
		REQUIRE(T::table_from_base['M'] == 12);
		REQUIRE(T::table_from_base['N'] == 13);
		REQUIRE(T::table_from_base['O'] == 14);
		REQUIRE(T::table_from_base['P'] == 15);
		REQUIRE(T::table_from_base['Q'] == 16);
		REQUIRE(T::table_from_base['R'] == 17);
		REQUIRE(T::table_from_base['S'] == 18);
		REQUIRE(T::table_from_base['T'] == 19);
		REQUIRE(T::table_from_base['U'] == 20);
		REQUIRE(T::table_from_base['V'] == 21);
		REQUIRE(T::table_from_base['W'] == 22);
		REQUIRE(T::table_from_base['X'] == 23);
		REQUIRE(T::table_from_base['Y'] == 24);
		REQUIRE(T::table_from_base['Z'] == 25);

		REQUIRE(T::table_from_base['a'] == 26);
		REQUIRE(T::table_from_base['b'] == 27);
		REQUIRE(T::table_from_base['c'] == 28);
		REQUIRE(T::table_from_base['d'] == 29);
		REQUIRE(T::table_from_base['e'] == 30);
		REQUIRE(T::table_from_base['f'] == 31);
		REQUIRE(T::table_from_base['g'] == 32);
		REQUIRE(T::table_from_base['h'] == 33);
		REQUIRE(T::table_from_base['i'] == 34);
		REQUIRE(T::table_from_base['j'] == 35);
		REQUIRE(T::table_from_base['k'] == 36);
		REQUIRE(T::table_from_base['l'] == 37);
		REQUIRE(T::table_from_base['m'] == 38);
		REQUIRE(T::table_from_base['n'] == 39);
		REQUIRE(T::table_from_base['o'] == 40);
		REQUIRE(T::table_from_base['p'] == 41);
		REQUIRE(T::table_from_base['q'] == 42);
		REQUIRE(T::table_from_base['r'] == 43);
		REQUIRE(T::table_from_base['s'] == 44);
		REQUIRE(T::table_from_base['t'] == 45);
		REQUIRE(T::table_from_base['u'] == 46);
		REQUIRE(T::table_from_base['v'] == 47);
		REQUIRE(T::table_from_base['w'] == 48);
		REQUIRE(T::table_from_base['x'] == 49);
		REQUIRE(T::table_from_base['y'] == 50);
		REQUIRE(T::table_from_base['z'] == 51);

		REQUIRE(T::table_from_base['0'] == 52);
		REQUIRE(T::table_from_base['1'] == 53);
		REQUIRE(T::table_from_base['2'] == 54);
		REQUIRE(T::table_from_base['3'] == 55);
		REQUIRE(T::table_from_base['4'] == 56);
		REQUIRE(T::table_from_base['5'] == 57);
		REQUIRE(T::table_from_base['6'] == 58);
		REQUIRE(T::table_from_base['7'] == 59);
		REQUIRE(T::table_from_base['8'] == 60);
		REQUIRE(T::table_from_base['9'] == 61);

		REQUIRE(T::table_from_base[u62] == 62);
		REQUIRE(T::table_from_base[u63] == 63);
	}
	LINGO_WARNINGS_POP_CLANG
}

TEST_CASE("base64 uses the correct tables")
{
	test_to_base_base64<lingo::encoding::base64<char, char>>('+', '/');
	test_from_base_base64<lingo::encoding::base64<char, char>>('+', '/');
}

TEST_CASE("strings can be encoded and decoded into base64")
{
	using base64_string_type = lingo::basic_string<lingo::encoding::join<lingo::encoding::base64<char, char>, lingo::encoding::utf8<char, char32_t>>, lingo::page::unicode>;
	using utf8_string_type = lingo::utf8_string;
	
	const utf8_string_type utf8_string(u8"LoremЛоремसादगिالمنتصر🧥👚👕👖👔");
	const base64_string_type base64_string(utf8_string);
}