header_open = """#ifndef H_LINGO_TEST_STRINGS
#define H_LINGO_TEST_STRINGS

#include <array>
#include <cstddef>
#include <type_traits>

namespace lingo
{
	namespace test
	{
		template <typename UnitT>
		struct test_string
		{
			const UnitT* data;
			std::size_t size;
		};

		template <typename Encoding, typename CharacterSet, typename = void>
		struct test_strings;
"""

header_close = """	}
}

#endif"""

types = ["char", "char16_t", "char32_t"]
encodings = ["utf8"]
sets = ["unicode"]

def main():
	test_strings = ["a", "b", "cfdgdfgfdgdfgfg"]

	all_unicode_characters = ""
	for i in range(1, 0x110000):
		code_point = chr(i)
		all_unicode_characters = all_unicode_characters + code_point
	test_strings.append(all_unicode_characters)

	with open("_.hpp", "w+") as header:
		header.write(header_open)

		for character_set in sets:
			for encoding in encodings:
				for point_type in types:
					for unit_type in types:
						header.write("\n\t\ttemplate <>\n\t\tstruct test_strings<lingo::encoding::{0}<{1}, {2}>, lingo::set::{3}, std::enable_if<sizeof({1}) <= sizeof({2})>::type>\n".format(encoding, unit_type, point_type, character_set))
						header.write("\t\t{\n")
						for i in range(0, len(test_strings)):
							test_string = test_strings[i].encode("utf-8", errors="surrogatepass")
							header.write("\t\t\tconstexpr test_string{} = {{ ".format(i))
							for byte in test_string:
								header.write("({}){}, ".format(unit_type, byte))
							header.write("};\n")
						header.write("\n\t\t\tconstexpr std::array<test_string, {}> =\n".format(len(test_strings)))
						header.write("\t\t\t{\n")
						for i in range(0, len(test_strings)):
							header.write("\t\t\t\t{{ test_string{}, {} }},\n".format(i, i))
						header.write("\t\t\t};\n")
						header.write("\t\t};\n")
		header.write(header_close)

if __name__ == "__main__":
	main()