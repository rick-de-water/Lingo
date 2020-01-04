#ifndef H_LINGO_ERROR_STRICT
#define H_LINGO_ERROR_STRICT

#include <lingo/constexpr.hpp>
#include <lingo/conversion_result.hpp>

#include <lingo/error/error_code.hpp>
#include <lingo/error/exception.hpp>

#include <cstddef>

namespace lingo
{
	namespace error
	{
		template <typename SourceEncoding, typename SourcePage, typename DestinationEncoding, typename DestinationPage>
		struct strict
		{
			static LINGO_CONSTEXPR14 conversion_result handle(
				const typename SourceEncoding::unit_type*, std::size_t,
				typename DestinationEncoding::unit_type*, std::size_t,
				error_code err)
			{
				return false ? conversion_result{} : throw exception(err);
			}
		};
	}
}

#endif