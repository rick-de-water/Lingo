#ifndef H_LINGO_SET_UNICODE
#define H_LINGO_SET_UNICODE

#include <lingo/constexpr.hpp>

#include <lingo/set/result.hpp>

#include <cstddef>

namespace lingo
{
	namespace set
	{
		template <typename PointT = char32_t>
		struct unicode
		{
			using point_type = PointT;

			static LINGO_CONSTEXPR11 size_t size() noexcept
			{
				return 0x110000;
			}

			static LINGO_CONSTEXPR11 to_unicode_result to_unicode(point_type point)
			{
				return to_unicode_result{ point, error::error_code::success };
			}

			static LINGO_CONSTEXPR11 from_unicode_result<PointT> from_unicode(char32_t point)
			{
				return from_unicode_result<PointT>{ point, error::error_code::success };
			}
		};
	}
}

#endif