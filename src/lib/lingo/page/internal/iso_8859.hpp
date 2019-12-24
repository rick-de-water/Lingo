#ifndef H_LINGO_PAGE_INTERNAL_ISO_8859
#define H_LINGO_PAGE_INTERNAL_ISO_8859

#include <lingo/constexpr.hpp>

#include <lingo/page/unicode.hpp>

#include <type_traits>

namespace lingo
{
	namespace page
	{
		namespace internal
		{
			template <typename ToUnicodeMapping, typename FromUnicodeMapping>
			struct iso_8859
			{
				private:
				using to_unicode_mapping_type = ToUnicodeMapping;
				using from_unicode_mapping_type = FromUnicodeMapping;

				public:
				using point_type = unsigned char;

				static LINGO_CONSTEXPR11 std::size_t point_range = 256;

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

				template <typename SourcePage>
				static LINGO_CONSTEXPR14 auto map_from(typename SourcePage::point_type point) noexcept ->
					typename std::enable_if<
						std::is_same<SourcePage, unicode>::value,
						map_result<point_type>>::type
				{
					const uint_least32_t point_index = static_cast<uint_least32_t>(point);

					LINGO_CONSTEXPR11 uint_least32_t minor_table_size = sizeof(to_unicode_mapping_type::minor_table_empty) / sizeof(to_unicode_mapping_type::minor_table_empty[0]);
					const uint_least32_t major_table_index = point_index / minor_table_size;

					if (major_table_index < to_unicode_mapping_type::minor_table_start_index || major_table_index >= to_unicode_mapping_type::minor_table_end_index)
					{
						return { {}, error::error_code::no_mapping };
					}

					const uint_least32_t minor_table_index = point_index - (major_table_index * minor_table_size);
					const uint_least32_t mapped_point = to_unicode_mapping_type::major_table[major_table_index][minor_table_index];

					if (mapped_point == 0xFFFF)
					{
						return { {}, error::error_code::no_mapping };
					}

					return { static_cast<point_type>(mapped_point), error::error_code::success };
				}
			};
		}
	}
}

#endif