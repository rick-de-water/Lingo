#ifndef H_LINGO_STRLEN
#define H_LINGO_STRLEN

#include <lingo/platform/constexpr.hpp>

#include <cstddef>

namespace lingo
{
	template <typename Unit>
	LINGO_CONSTEXPR14 std::size_t strlen(const Unit* string) noexcept
	{
		const Unit* null_character = string;
		while (*null_character)
		{
			++null_character;
		}

		return null_character - string;
	}

	template <typename Unit>
	LINGO_CONSTEXPR14 std::size_t strlen_s(const Unit* string, std::size_t maximum_size) noexcept
	{
		if (string == nullptr)
		{
			return 0;
		}

		for (size_t i = 0; i < maximum_size; ++i)
		{
			if (string[i] == 0)
			{
				return i;
			}
		}

		return maximum_size;
	}
}

#endif