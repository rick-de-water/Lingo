#ifndef H_LINGO_ENCODING_UTF16
#define H_LINGO_ENCODING_UTF16

#include <lingo/constexpr.hpp>

#include <lingo/encoding/result.hpp>

#include <cassert>
#include <climits>
#include <limits>
#include <type_traits>

namespace lingo
{
	namespace encoding
	{
		template <typename Unit, typename Point>
		struct utf16
		{
			public:
			using unit_type = Unit;
			using point_type = Point;

			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;

			private:
			static_assert(sizeof(unit_type) * CHAR_BIT >= 16, "A single unit must have at least 16 bits");

			public:
			static LINGO_CONSTEXPR11 size_type max_units = 4;

			static LINGO_CONSTEXPR14 size_type point_size(point_type point) noexcept
			{
				return 0;
			}

			static LINGO_CONSTEXPR14 size_type unit_size(unit_type unit) noexcept
			{
				return 0;
			}

			static LINGO_CONSTEXPR14 encode_result encode_point(point_type point, unit_type* buffer, size_type buffer_size) noexcept
			{
				return {};
			}

			static LINGO_CONSTEXPR14 decode_result<point_type> decode_point(const unit_type* buffer, size_type buffer_size) noexcept
			{
				return {};
			}
		};
	}
}

#endif