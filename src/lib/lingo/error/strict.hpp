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
			static LINGO_CONSTEXPR11 encoding::encode_result handle(
				const encoding::encode_result& result,
				utility::span<const typename Encoding::point_type>,
				utility::span<typename Encoding::unit_type>)
			{
				return result.error == error::error_code::success ? result : throw exception(result.error);
			}

			static LINGO_CONSTEXPR11 encoding::decode_result handle(
				const encoding::decode_result& result,
				utility::span<const typename Encoding::unit_type>,
				utility::span<typename Encoding::point_type>)
			{
				return result.error == error::error_code::success ? result : throw exception(result.error);
			}
		};
	}
}

#endif