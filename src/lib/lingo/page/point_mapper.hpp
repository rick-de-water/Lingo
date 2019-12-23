#ifndef H_LINGO_PAGE_POINT_MAPPER
#define H_LINGO_PAGE_POINT_MAPPER

#include <lingo/constexpr.hpp>

#include <lingo/page/result.hpp>
#include <lingo/page/intermediate.hpp>

#include <type_traits>

#define LINGO_POINT_MAPPER_TYPEDEFS \
	using source_page_type = SourcePage; \
	using destination_page_type = DestinationPage; \
	using source_point_type = typename source_page_type::point_type; \
	using destination_point_type = typename destination_page_type::point_type; \
	using result_type = map_result<destination_point_type>

namespace lingo
{
	namespace page
	{
		// Default implementation uses an intermediate code page
		template <typename SourcePage, typename DestinationPage, typename IntermediatePage = typename intermediate<SourcePage, DestinationPage>::type, typename Enable = void>
		struct point_mapper
		{
			LINGO_POINT_MAPPER_TYPEDEFS;

			static LINGO_CONSTEXPR14 result_type map(source_point_type source_point) noexcept
			{
				const auto to_intermediate_result = source_page_type::template map_to<IntermediatePage>(source_point);
				if (to_intermediate_result.error != error::error_code::success)
				{
					return { {}, to_intermediate_result.error };
				}

				return destination_page_type::template map_from<IntermediatePage>(to_intermediate_result.point);
			}
		};

		// No conversion is needed when the source and destination pages are the same
		template <typename SourcePage, typename DestinationPage, typename IntermediatePage>
		struct point_mapper<SourcePage, DestinationPage, IntermediatePage,
			typename std::enable_if<
				std::is_same<SourcePage, DestinationPage>::value>::type>
		{
			LINGO_POINT_MAPPER_TYPEDEFS;

			static LINGO_CONSTEXPR14 result_type map(source_point_type source_point) noexcept
			{
				return { source_point, error::error_code::success };
			}
		};

		// When the source page is the same as the intermediate page we only need to convert the destination
		template <typename SourcePage, typename DestinationPage, typename IntermediatePage>
		struct point_mapper<SourcePage, DestinationPage, IntermediatePage,
			typename std::enable_if<
				!std::is_same<SourcePage, DestinationPage>::value &&
				std::is_same<SourcePage, IntermediatePage>::value>::type>
		{
			LINGO_POINT_MAPPER_TYPEDEFS;

			static LINGO_CONSTEXPR14 result_type map(source_point_type source_point) noexcept
			{
				return destination_page_type::template map_from<IntermediatePage>(source_point);
			}
		};

		// When the destination page is the same as the intermediate page we only need to convert the source
		template <typename SourcePage, typename DestinationPage, typename IntermediatePage>
		struct point_mapper<SourcePage, DestinationPage, IntermediatePage,
			typename std::enable_if<
			!std::is_same<SourcePage, DestinationPage>::value &&
			std::is_same<DestinationPage, IntermediatePage>::value>::type>
		{
			LINGO_POINT_MAPPER_TYPEDEFS;

			static LINGO_CONSTEXPR14 result_type map(source_point_type source_point) noexcept
			{
				return source_page_type::template map_to<IntermediatePage>(source_point);
			}
		};
	}
}

#undef LINGO_POINT_MAPPER_TYPEDEFS

#endif