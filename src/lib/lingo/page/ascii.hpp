#ifndef H_LINGO_PAGE_ASCII
#define H_LINGO_PAGE_ASCII

#include <lingo/constexpr.hpp>

#include <lingo/page/unicode.hpp>

#include <cstddef>

namespace lingo
{
	namespace page
	{
		struct ascii
		{
			using point_type = char;

			static LINGO_CONSTEXPR11 size_t size() noexcept
			{
				return 128;
			}

			template <typename SourcePage>
			static LINGO_CONSTEXPR14 auto map_from(typename SourcePage::point_type point) noexcept ->
				typename std::enable_if<
					std::is_same<SourcePage, unicode>::value,
					map_result<point_type>>::type
			{
				if (point >= 0 && point < 128)
				{
					return { static_cast<point_type>(point), error::error_code::success };
				}
				else
				{
					return { {}, error::error_code::no_mapping };
				}
			}

			template <typename DestinationPage>
			static LINGO_CONSTEXPR14 auto map_to(point_type point) noexcept ->
				typename std::enable_if<
					std::is_same<DestinationPage, unicode>::value,
					map_result<typename DestinationPage::point_type>>::type
			{
				if (point >= 0 && point < 128)
				{
					return { static_cast<typename DestinationPage::point_type>(point), error::error_code::success };
				}
				else
				{
					return { {}, error::error_code::no_mapping };
				}
			}
		};
	}
}

#endif