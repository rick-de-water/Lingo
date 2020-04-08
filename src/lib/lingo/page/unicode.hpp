#ifndef H_LINGO_PAGE_UNICODE
#define H_LINGO_PAGE_UNICODE

#include <lingo/platform/constexpr.hpp>

#include <lingo/page/result.hpp>

#include <lingo/page/unicode_version.hpp>

#include <cstddef>
#include <type_traits>

namespace lingo
{
	namespace page
	{
		template <unicode::version version>
		struct basic_unicode
		{
			using point_type = char32_t;

			static LINGO_CONSTEXPR11 size_t point_range = 0x110000;

			template <typename SourcePage>
			static LINGO_CONSTEXPR14 auto map_from(typename SourcePage::point_type point) ->
				typename std::enable_if<
				std::is_same<SourcePage, basic_unicode>::value,
				map_result<point_type>>::type
			{
				return map_result<point_type>{ point, error::error_code::success };
			}

				template <typename DestinationPage>
			static LINGO_CONSTEXPR14 auto map_to(point_type point) ->
				typename std::enable_if<
				std::is_same<DestinationPage, basic_unicode>::value,
				map_result<typename DestinationPage::point_type>>::type
			{
				return map_result<typename DestinationPage::point_type>{ point, error::error_code::success };
			}
		};

		using unicode_v1_1 = basic_unicode<unicode::version::v1_1>;
		using unicode_v2_0 = basic_unicode<unicode::version::v2_0>;
		using unicode_v2_1 = basic_unicode<unicode::version::v2_1>;
		using unicode_v3_0 = basic_unicode<unicode::version::v3_0>;
		using unicode_v3_1 = basic_unicode<unicode::version::v3_1>;
		using unicode_v3_2 = basic_unicode<unicode::version::v3_2>;
		using unicode_v4_0 = basic_unicode<unicode::version::v4_0>;
		using unicode_v4_1 = basic_unicode<unicode::version::v4_1>;
		using unicode_v5_0 = basic_unicode<unicode::version::v5_0>;
		using unicode_v5_1 = basic_unicode<unicode::version::v5_1>;
		using unicode_v5_2 = basic_unicode<unicode::version::v5_2>;
		using unicode_v6_0 = basic_unicode<unicode::version::v6_0>;
		using unicode_v6_1 = basic_unicode<unicode::version::v6_1>;
		using unicode_v6_2 = basic_unicode<unicode::version::v6_2>;
		using unicode_v6_3 = basic_unicode<unicode::version::v6_3>;
		using unicode_v7_0 = basic_unicode<unicode::version::v7_0>;
		using unicode_v8_0 = basic_unicode<unicode::version::v8_0>;
		using unicode_v9_0 = basic_unicode<unicode::version::v9_0>;
		using unicode_v10_0 = basic_unicode<unicode::version::v10_0>;
		using unicode_v11_0 = basic_unicode<unicode::version::v11_0>;
		using unicode_v12_0 = basic_unicode<unicode::version::v12_0>;
		using unicode_v12_1 = basic_unicode<unicode::version::v12_1>;

		using unicode_latest = unicode_v12_1;
		using unicode_default = unicode_latest;
	}

	namespace utility
	{
		template <typename Page>
		struct is_unicode : public std::false_type
		{
		};

		template <page::unicode::version version>
		struct is_unicode<page::basic_unicode<version>> : public std::true_type
		{
		};

		#ifdef __cpp_variable_templates
		template <typename Page>
		LINGO_CONSTEXPR14 const bool is_unicode_v = is_unicode<Page>::value;
		#endif
	}
}

#endif