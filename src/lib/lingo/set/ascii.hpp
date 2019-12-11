#ifndef H_LINGO_SETS_ASCII
#define H_LINGO_SETS_ASCII

#include <lingo/constexpr.hpp>

#include <cstddef>

namespace lingo
{
	namespace set
	{
		struct ascii
		{
			static LINGO_CONSTEXPR11 size_t size() noexcept
			{
				return 128;
			}
		};
	}
}

#endif