#ifndef H_LINGO_ERROR_STRICT
#define H_LINGO_ERROR_STRICT

#include <lingo/platform/constexpr.hpp>
#include <lingo/conversion_result.hpp>

#include <lingo/error/error_code.hpp>
#include <lingo/error/exception.hpp>

#include <lingo/utility/span.hpp>

#include <cstddef>

namespace lingo
{
	namespace error
	{
		template <typename Encoding, typename Page>
		struct strict
		{
			using encoding_type = Encoding;
			using page_type = Page;

			using unit_type = typename encoding_type::unit_type;
			using point_type = typename encoding_type::point_type;

			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;

			using encode_result_type = typename encoding_type::encode_result_type;
			using decode_result_type = typename encoding_type::decode_result_type;
			using encode_source_type = typename encoding_type::encode_source_type;
			using decode_source_type = typename encoding_type::decode_source_type;
			using encode_destination_type = typename encoding_type::encode_destination_type;
			using decode_destination_type = typename encoding_type::decode_destination_type;

			static LINGO_CONSTEXPR11 encode_result_type handle(
				const encode_result_type& result,
				encode_source_type,
				encode_destination_type)
			{
				return result.error == error::error_code::success ? result : throw exception(result.error);
			}

			static LINGO_CONSTEXPR11 decode_result_type handle(
				const decode_result_type& result,
				decode_source_type,
				decode_destination_type)
			{
				return result.error == error::error_code::success ? result : throw exception(result.error);
			}
		};
	}
}

#endif