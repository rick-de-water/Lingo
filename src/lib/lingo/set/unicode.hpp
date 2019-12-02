#ifndef H_LINGO_SETS_UNICODE
#define H_LINGO_SETS_UNICODE

#include <lingo/constexpr.hpp>

#include <cstddef>

namespace lingo
{
	namespace set
	{
		struct unicode
		{
			static LINGO_CONSTEXPR11 size_t size()
			{
				return 0x110000;
			}
		};
	}
}

#endif