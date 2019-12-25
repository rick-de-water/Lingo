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

				using to_unicode_point_type = typename to_unicode_mapping_type::point_type;
				using from_unicode_point_type = typename from_unicode_mapping_type::point_type;

				public:
				using point_type = unsigned char;

				static LINGO_CONSTEXPR11 std::size_t point_range = 256;

				template <typename DestinationPage>
				static LINGO_CONSTEXPR14 auto map_to(point_type point) noexcept ->
					typename std::enable_if<
					std::is_same<DestinationPage, unicode>::value,
					map_result<typename DestinationPage::point_type>>::type
				{
					const to_unicode_point_type mapped_point = map<to_unicode_mapping_type>(static_cast<to_unicode_point_type>(point));
					if (mapped_point != 0xFFFF)
					{
						return { static_cast<typename DestinationPage::point_type>(mapped_point), error::error_code::success };
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
					const from_unicode_point_type mapped_point = map<from_unicode_mapping_type>(static_cast<from_unicode_point_type>(point));
					if (mapped_point != 0xFFFF)
					{
						return { static_cast<point_type>(mapped_point), error::error_code::success };
					}
					else
					{
						return { {}, error::error_code::no_mapping };
					}
				}

				private:
				template <typename Mapping, typename Mapping::point_type InvalidPoint = 0xFFFF>
				static LINGO_CONSTEXPR14 typename Mapping::point_type map(typename Mapping::point_type point)
				{
					const std::size_t point_index = static_cast<::size_t>(point);
					LINGO_CONSTEXPR11 std::size_t minor_table_size = sizeof(Mapping::minor_table_empty) / sizeof(Mapping::minor_table_empty[0]);
					const std::size_t major_table_index = point_index / minor_table_size;

					if (major_table_index < Mapping::minor_table_start_index || major_table_index >= Mapping::minor_table_end_index)
					{
						return InvalidPoint;
					}

					const std::size_t minor_table_index = point_index - (major_table_index * minor_table_size);
					const typename Mapping::point_type mapped_point = Mapping::major_table[major_table_index][minor_table_index];

					if (mapped_point == InvalidPoint)
					{
						return InvalidPoint;
					}

					return mapped_point;
				}
			};
		}
	}
}

#endif