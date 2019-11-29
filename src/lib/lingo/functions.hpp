#ifndef H_LINGO_FUNCTIONS
#define H_LINGO_FUNCTIONS

#include <cstddef>

namespace lingo
{
	template <typename CharT>
	std::size_t strlen(const CharT* string)
	{
		const CharT* null_character = string;
		while (*null_character)
		{
			++null_character;
		}

		return null_character - string;
	}
}

#endif