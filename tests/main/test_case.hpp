#ifndef H_LINGO_TEST_CASE
#define H_LINGO_TEST_CASE

#include <catch/catch.hpp>

#include <lingo/string.hpp>
#include <lingo/string_view.hpp>

#include <lingo/encoding/utf8.hpp>

#include <lingo/page/unicode.hpp>

#include <test_types.hpp>

#include <cstddef>
#include <memory>
#include <tuple>
#include <type_traits>
#include <vector>

namespace lingo
{
	namespace test
	{
		template <typename Unit>
		struct cstring
		{
			const Unit* data;
			std::size_t size;
			std::unique_ptr<Unit[]> buffer;
		};

		template <typename Encoding>
		std::vector<cstring<typename Encoding::unit_type>> generate_test_strings()
		{
			using encoding_type = Encoding;
			using unit_type = typename encoding_type::unit_type;
			using point_type = typename encoding_type::point_type;

			std::vector<cstring<unit_type>> test_strings;

			for (std::size_t i = 0; i < 10; ++i)
			{
				cstring<unit_type> str{};
				str.buffer = std::make_unique<unit_type[]>(encoding_type::max_units * i + 1);
				str.size = 0;

				for (point_type c = 0; c < point_type(i); ++c)
				{
					str.size += encoding_type::encode_point(static_cast<point_type>(U'a') + c, str.buffer.get() + str.size, encoding_type::max_units).size;
				}

				str.buffer[str.size] = 0;
				str.data = str.buffer.get();
				test_strings.emplace_back(std::move(str));
			}

			return test_strings;
		}

		using test_types = lingo::test::tuple_matrix_t<lingo::test::unit_types, lingo::test::unit_types>;
	}
}

#define LINGO_TEST_CASE(...) TEMPLATE_LIST_TEST_CASE(__VA_ARGS__, "", lingo::test::unit_types)

#define LINGO_TEST_TYPEDEFS \
	using unit_type = TestType; \
	using point_type = char32_t; \
	using encoding_type = lingo::encoding::utf8<unit_type, point_type>; \
	using page_type = lingo::page::unicode; \
	using string = lingo::basic_string<encoding_type, page_type>; \
	using string_view = lingo::basic_string_view<encoding_type, page_type>; \
	using size_type = typename string::size_type; \
	using difference_type = typename string::difference_type

#endif