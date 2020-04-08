#ifndef H_LINGO_PAGE_ISO_8859
#define H_LINGO_PAGE_ISO_8859

#include <lingo/platform/constexpr.hpp>
#include <lingo/page/unicode.hpp>
#include <lingo/utility/type_traits.hpp>

#include <lingo/page/internal/iso_8859_1_unicode_mapping.hpp>
#include <lingo/page/internal/iso_8859_2_unicode_mapping.hpp>
#include <lingo/page/internal/iso_8859_3_unicode_mapping.hpp>
#include <lingo/page/internal/iso_8859_4_unicode_mapping.hpp>
#include <lingo/page/internal/iso_8859_5_unicode_mapping.hpp>
#include <lingo/page/internal/iso_8859_6_unicode_mapping.hpp>
#include <lingo/page/internal/iso_8859_7_unicode_mapping.hpp>
#include <lingo/page/internal/iso_8859_8_unicode_mapping.hpp>
#include <lingo/page/internal/iso_8859_9_unicode_mapping.hpp>
#include <lingo/page/internal/iso_8859_10_unicode_mapping.hpp>
#include <lingo/page/internal/iso_8859_11_unicode_mapping.hpp>
#include <lingo/page/internal/iso_8859_13_unicode_mapping.hpp>
#include <lingo/page/internal/iso_8859_14_unicode_mapping.hpp>
#include <lingo/page/internal/iso_8859_15_unicode_mapping.hpp>
#include <lingo/page/internal/iso_8859_16_unicode_mapping.hpp>

#include <lingo/page/internal/unicode_iso_8859_1_mapping.hpp>
#include <lingo/page/internal/unicode_iso_8859_2_mapping.hpp>
#include <lingo/page/internal/unicode_iso_8859_3_mapping.hpp>
#include <lingo/page/internal/unicode_iso_8859_4_mapping.hpp>
#include <lingo/page/internal/unicode_iso_8859_5_mapping.hpp>
#include <lingo/page/internal/unicode_iso_8859_6_mapping.hpp>
#include <lingo/page/internal/unicode_iso_8859_7_mapping.hpp>
#include <lingo/page/internal/unicode_iso_8859_8_mapping.hpp>
#include <lingo/page/internal/unicode_iso_8859_9_mapping.hpp>
#include <lingo/page/internal/unicode_iso_8859_10_mapping.hpp>
#include <lingo/page/internal/unicode_iso_8859_11_mapping.hpp>
#include <lingo/page/internal/unicode_iso_8859_13_mapping.hpp>
#include <lingo/page/internal/unicode_iso_8859_14_mapping.hpp>
#include <lingo/page/internal/unicode_iso_8859_15_mapping.hpp>
#include <lingo/page/internal/unicode_iso_8859_16_mapping.hpp>

#include <cstddef>
#include <type_traits>

namespace lingo
{
	namespace page
	{
		namespace internal
		{
			template <typename ToUnicodeMapping, typename FromUnicodeMapping>
			struct iso_8859_impl
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
					utility::is_unicode<DestinationPage>::value,
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
						utility::is_unicode<SourcePage>::value,
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

		template <std::size_t Part, typename = void>
		struct iso_8859;

		template <typename _>
		struct iso_8859<1, _> : public internal::iso_8859_impl<
			internal::iso_8859_1_unicode_mapping<unicode_default::point_type>,
			internal::unicode_iso_8859_1_mapping<unicode_default::point_type>>
		{
			static LINGO_CONSTEXPR11 std::size_t part_index = 1;
		};

		template <typename _>
		struct iso_8859<2, _> : public internal::iso_8859_impl<
			internal::iso_8859_2_unicode_mapping<unicode_default::point_type>,
			internal::unicode_iso_8859_2_mapping<unicode_default::point_type>>
		{
			static LINGO_CONSTEXPR11 std::size_t part_index = 2;
		};

		template <typename _>
		struct iso_8859<3, _> : public internal::iso_8859_impl<
			internal::iso_8859_3_unicode_mapping<unicode_default::point_type>,
			internal::unicode_iso_8859_3_mapping<unicode_default::point_type>>
		{
			static LINGO_CONSTEXPR11 std::size_t part_index = 3;
		};

		template <typename _>
		struct iso_8859<4, _> : public internal::iso_8859_impl<
			internal::iso_8859_4_unicode_mapping<unicode_default::point_type>,
			internal::unicode_iso_8859_4_mapping<unicode_default::point_type>>
		{
			static LINGO_CONSTEXPR11 std::size_t part_index = 4;
		};

		template <typename _>
		struct iso_8859<5, _> : public internal::iso_8859_impl<
			internal::iso_8859_5_unicode_mapping<unicode_default::point_type>,
			internal::unicode_iso_8859_5_mapping<unicode_default::point_type>>
		{
			static LINGO_CONSTEXPR11 std::size_t part_index = 5;
		};

		template <typename _>
		struct iso_8859<6, _> : public internal::iso_8859_impl<
			internal::iso_8859_6_unicode_mapping<unicode_default::point_type>,
			internal::unicode_iso_8859_6_mapping<unicode_default::point_type>>
		{
			static LINGO_CONSTEXPR11 std::size_t part_index = 6;
		};

		template <typename _>
		struct iso_8859<7, _> : public internal::iso_8859_impl<
			internal::iso_8859_7_unicode_mapping<unicode_default::point_type>,
			internal::unicode_iso_8859_7_mapping<unicode_default::point_type>>
		{
			static LINGO_CONSTEXPR11 std::size_t part_index = 7;
		};

		template <typename _>
		struct iso_8859<8, _> : public internal::iso_8859_impl<
			internal::iso_8859_8_unicode_mapping<unicode_default::point_type>,
			internal::unicode_iso_8859_8_mapping<unicode_default::point_type>>
		{
			static LINGO_CONSTEXPR11 std::size_t part_index = 8;
		};

		template <typename _>
		struct iso_8859<9, _> : public internal::iso_8859_impl<
			internal::iso_8859_9_unicode_mapping<unicode_default::point_type>,
			internal::unicode_iso_8859_9_mapping<unicode_default::point_type>>
		{
			static LINGO_CONSTEXPR11 std::size_t part_index = 9;
		};

		template <typename _>
		struct iso_8859<10, _> : public internal::iso_8859_impl<
			internal::iso_8859_10_unicode_mapping<unicode_default::point_type>,
			internal::unicode_iso_8859_10_mapping<unicode_default::point_type>>
		{
			static LINGO_CONSTEXPR11 std::size_t part_index = 10;
		};

		template <typename _>
		struct iso_8859<11, _> : public internal::iso_8859_impl<
			internal::iso_8859_11_unicode_mapping<unicode_default::point_type>,
			internal::unicode_iso_8859_11_mapping<unicode_default::point_type>>
		{
			static LINGO_CONSTEXPR11 std::size_t part_index = 11;
		};

		template <typename _>
		struct iso_8859<13, _> : public internal::iso_8859_impl<
			internal::iso_8859_13_unicode_mapping<unicode_default::point_type>,
			internal::unicode_iso_8859_13_mapping<unicode_default::point_type>>
		{
			static LINGO_CONSTEXPR11 std::size_t part_index = 13;
		};

		template <typename _>
		struct iso_8859<14, _> : public internal::iso_8859_impl<
			internal::iso_8859_14_unicode_mapping<unicode_default::point_type>,
			internal::unicode_iso_8859_14_mapping<unicode_default::point_type>>
		{
			static LINGO_CONSTEXPR11 std::size_t part_index = 14;
		};

		template <typename _>
		struct iso_8859<15, _> : public internal::iso_8859_impl<
			internal::iso_8859_15_unicode_mapping<unicode_default::point_type>,
			internal::unicode_iso_8859_15_mapping<unicode_default::point_type>>
		{
			static LINGO_CONSTEXPR11 std::size_t part_index = 15;
		};

		template <typename _>
		struct iso_8859<16, _> : public internal::iso_8859_impl<
			internal::iso_8859_16_unicode_mapping<unicode_default::point_type>,
			internal::unicode_iso_8859_16_mapping<unicode_default::point_type>>
		{
			static LINGO_CONSTEXPR11 std::size_t part_index = 16;
		};

		template <typename _> LINGO_CONSTEXPR11 std::size_t iso_8859<1, _>::part_index;
		template <typename _> LINGO_CONSTEXPR11 std::size_t iso_8859<2, _>::part_index;
		template <typename _> LINGO_CONSTEXPR11 std::size_t iso_8859<3, _>::part_index;
		template <typename _> LINGO_CONSTEXPR11 std::size_t iso_8859<4, _>::part_index;
		template <typename _> LINGO_CONSTEXPR11 std::size_t iso_8859<5, _>::part_index;
		template <typename _> LINGO_CONSTEXPR11 std::size_t iso_8859<6, _>::part_index;
		template <typename _> LINGO_CONSTEXPR11 std::size_t iso_8859<7, _>::part_index;
		template <typename _> LINGO_CONSTEXPR11 std::size_t iso_8859<8, _>::part_index;
		template <typename _> LINGO_CONSTEXPR11 std::size_t iso_8859<9, _>::part_index;
		template <typename _> LINGO_CONSTEXPR11 std::size_t iso_8859<10, _>::part_index;
		template <typename _> LINGO_CONSTEXPR11 std::size_t iso_8859<11, _>::part_index;
		template <typename _> LINGO_CONSTEXPR11 std::size_t iso_8859<13, _>::part_index;
		template <typename _> LINGO_CONSTEXPR11 std::size_t iso_8859<14, _>::part_index;
		template <typename _> LINGO_CONSTEXPR11 std::size_t iso_8859<15, _>::part_index;
		template <typename _> LINGO_CONSTEXPR11 std::size_t iso_8859<16, _>::part_index;

		using iso_8859_1 = iso_8859<1>;
		using iso_8859_2 = iso_8859<2>;
		using iso_8859_3 = iso_8859<3>;
		using iso_8859_4 = iso_8859<4>;
		using iso_8859_5 = iso_8859<5>;
		using iso_8859_6 = iso_8859<6>;
		using iso_8859_7 = iso_8859<7>;
		using iso_8859_8 = iso_8859<8>;
		using iso_8859_9 = iso_8859<9>;
		using iso_8859_10 = iso_8859<10>;
		using iso_8859_11 = iso_8859<11>;
		using iso_8859_13 = iso_8859<13>;
		using iso_8859_14 = iso_8859<14>;
		using iso_8859_15 = iso_8859<15>;
		using iso_8859_16 = iso_8859<16>;
	}
}

#endif
