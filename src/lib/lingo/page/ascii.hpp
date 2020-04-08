#ifndef H_LINGO_PAGE_ASCII
#define H_LINGO_PAGE_ASCII

#include <lingo/platform/constexpr.hpp>
#include <lingo/page/unicode.hpp>

#include <cstddef>

namespace lingo
{
	namespace page
	{
		struct ascii
		{
			using point_type = char;

			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;

			static LINGO_CONSTEXPR11 size_type point_range = 128;

			template <typename DestinationPage>
			static LINGO_CONSTEXPR14 auto map_to(point_type point) noexcept ->
				typename std::enable_if<
					utility::is_unicode<DestinationPage>::value,
					map_result<typename DestinationPage::point_type>>::type
			{
				LINGO_WARNINGS_PUSH_AND_DISABLE_CLANG(tautological-constant-out-of-range-compare)
				if (point >= 0 && point < 128)
				LINGO_WARNINGS_POP_CLANG
				{
					return { static_cast<typename DestinationPage::point_type>(point), error::error_code::success };
				}
				else
				{
					return { {}, error::error_code::no_mapping };
				}
			}

			template <typename SourcePage>
			static LINGO_CONSTEXPR14 auto map_from(typename SourcePage::point_type point) noexcept ->
				typename std::enable_if<
					utility::is_unicode<SourcePage>::value,
					map_result<point_type>>::type
			{
				LINGO_WARNINGS_PUSH_AND_DISABLE_CLANG(tautological-constant-out-of-range-compare)
				if (point >= 0 && point < 128)
				LINGO_WARNINGS_POP_CLANG
				{
					return { static_cast<point_type>(point), error::error_code::success };
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