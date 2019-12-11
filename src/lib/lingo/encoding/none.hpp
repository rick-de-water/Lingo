#ifndef H_LINGO_ENCODING_NONE
#define H_LINGO_ENCODING_NONE

namespace lingo
{
	namespace encoding
	{
		template <typename UnitT, typename PointT>
		struct none
		{
			using unit_type = UnitT;
			using point_type = PointT;

			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;

			static LINGO_CONSTEXPR11 size_type max_point_size = 1;

			static LINGO_CONSTEXPR14 size_type point_size(point_type point) noexcept
			{
				return 1;
			}

			static LINGO_CONSTEXPR14 size_type unit_size(unit_type unit) noexcept
			{
				return 1;
			}

			static LINGO_CONSTEXPR14 size_type encode_point(point_type point, unit_type* buffer, size_type buffer_size) noexcept
			{
				if (buffer_size == 0)
				{
					return 0;
				}

				buffer[0] = point;

				return 1;
			}

			static LINGO_CONSTEXPR14 point_type decode_point(const unit_type* buffer, size_type buffer_size) noexcept
			{
				if (buffer_size == 0)
				{
					return 0;
				}

				return buffer[0];
			}
		};
	}
}

#endif