#ifndef H_LINGO_PAGE_UNICODE
#define H_LINGO_PAGE_UNICODE

#include <lingo/constexpr.hpp>

#include <lingo/page/result.hpp>

#include <cstddef>

namespace lingo
{
	namespace page
	{
		struct unicode
		{
			using point_type = char32_t;

			static LINGO_CONSTEXPR11 size_t size() noexcept
			{
				return 0x110000;
			}

			template <typename DestinationPage>
			static LINGO_CONSTEXPR11 to_unicode_result to_unicode(point_type point)
			{

			}

			static LINGO_CONSTEXPR11 to_unicode_result to_unicode(point_type point)
			{
				return to_unicode_result{ point, error::error_code::success };
			}

			static LINGO_CONSTEXPR11 from_unicode_result<point_type> from_unicode(char32_t point)
			{
				return from_unicode_result<point_type>{ point, error::error_code::success };
			}
		};
	}
}

#endif