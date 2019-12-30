#ifndef H_LINGO_PAGE_UNICODE
#define H_LINGO_PAGE_UNICODE

#include <lingo/constexpr.hpp>

#include <lingo/page/result.hpp>

#include <cstddef>
#include <type_traits>

namespace lingo
{
	namespace page
	{
		struct unicode
		{
			using point_type = char32_t;

			static LINGO_CONSTEXPR11 size_t point_range = 0x110000;

			template <typename SourcePage>
			static LINGO_CONSTEXPR14 auto map_from(typename SourcePage::point_type point) ->
				typename std::enable_if<
					std::is_same<SourcePage, unicode>::value,
					map_result<point_type>>::type
			{
				return map_result<point_type>{ point, error::error_code::success };
			}

			template <typename DestinationPage>
			static LINGO_CONSTEXPR14 auto map_to(point_type point) ->
				typename std::enable_if<
					std::is_same<DestinationPage, unicode>::value,
					map_result<typename DestinationPage::point_type>>::type
			{
				return map_result<typename DestinationPage::point_type>{ point, error::error_code::success };
			}
		};
	}
}

#endif