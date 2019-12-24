#ifndef H_LINGO_PAGE_ISO_8859_1
#define H_LINGO_PAGE_ISO_8859_1

#include <lingo/constexpr.hpp>

#include <lingo/page/unicode.hpp>

#include <lingo/page/internal/iso_8859_1_unicode_mapping.hpp>
#include <lingo/page/internal/unicode_iso_8859_1_mapping.hpp>

#include <type_traits>

namespace lingo
{
	namespace page
	{
		struct iso_8859_1
		{
			using point_type = unsigned char;

			static LINGO_CONSTEXPR11 std::size_t size() noexcept
			{
				return 256;
			}

			template <typename SourcePage>
			static LINGO_CONSTEXPR14 auto map_from(typename SourcePage::point_type point) noexcept ->
				typename std::enable_if<
					std::is_same<SourcePage, unicode>::value,
					map_result<point_type>>::type
			{
				const uint_least32_t point_index = static_cast<uint_least32_t>(point);

				LINGO_CONSTEXPR11 uint_least32_t minor_table_size = sizeof(internal::minor_table_unicode_iso_8859_1_empty) / sizeof(internal::minor_table_unicode_iso_8859_1_empty[0]);
				const uint_least32_t major_table_index = point_index / minor_table_size;

				if (major_table_index < internal::minor_table_start_index_unicode_iso_8859_1 || major_table_index >= internal::minor_table_end_index_unicode_iso_8859_1)
				{
					return { {}, error::error_code::no_mapping };
				}

				const uint_least32_t minor_table_index = point_index - (major_table_index * minor_table_size);
				const uint_least32_t mapped_point = internal::major_table_unicode_iso_8859_1[major_table_index][minor_table_index];

				if (mapped_point == 0xFFFF)
				{
					return { {}, error::error_code::no_mapping };
				}

				return { static_cast<point_type>(mapped_point), error::error_code::success };
			}

			template <typename DestinationPage>
			static LINGO_CONSTEXPR14 auto map_to(point_type point) noexcept ->
				typename std::enable_if<
					std::is_same<DestinationPage, unicode>::value,
					map_result<typename DestinationPage::point_type>>::type
			{
				if (point >= 0 && point < 256)
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