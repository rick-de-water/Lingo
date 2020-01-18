#ifndef H_LINGO_TEST_CASE
#define H_LINGO_TEST_CASE

#include <catch/catch.hpp>

#include <lingo/string.hpp>
#include <lingo/string_view.hpp>

#include <lingo/encoding/execution.hpp>
#include <lingo/encoding/point_iterator.hpp>

#include <lingo/page/execution.hpp>

#include "test_types.hpp"

#include <cstddef>
#include <memory>
#include <tuple>
#include <type_traits>
#include <vector>

#define LINGO_UNIT_LEAST_64_TEST_CASE(...) TEMPLATE_LIST_TEST_CASE(__VA_ARGS__, "", lingo::test::unit_least_64_types)
#define LINGO_UNIT_LEAST_32_TEST_CASE(...) TEMPLATE_LIST_TEST_CASE(__VA_ARGS__, "", lingo::test::unit_least_32_types)
#define LINGO_UNIT_LEAST_16_TEST_CASE(...) TEMPLATE_LIST_TEST_CASE(__VA_ARGS__, "", lingo::test::unit_least_16_types)
#define LINGO_UNIT_LEAST_8_TEST_CASE(...) TEMPLATE_LIST_TEST_CASE(__VA_ARGS__, "", lingo::test::unit_least_8_types)
#define LINGO_UNIT_TEST_CASE(...) TEMPLATE_LIST_TEST_CASE(__VA_ARGS__, "", lingo::test::unit_types)

#define LINGO_UNIT_TEST_TYPEDEFS \
	using unit_type = TestType; \
	using encoding_type = lingo::encoding::execution_encoding_t<unit_type>; \
	using page_type = lingo::page::execution_page_t<unit_type>; \
	using point_type = typename page_type::point_type; \
	using size_type = std::size_t; \
	using difference_type = std::ptrdiff_t; \
	using allocator_type = lingo::internal::default_allocator<encoding_type>; \
	using string_type = lingo::basic_string<encoding_type, page_type>; \
	using string_view_type = lingo::basic_string_view<encoding_type, page_type>; \
	using point_iterator_type = lingo::encoding::point_iterator<encoding_type>

#endif