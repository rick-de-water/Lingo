#ifndef H_LINGO_STANDARD_TYPEDEFS
#define H_LINGO_STANDARD_TYPEDEFS

#include <cstddef>

namespace lingo
{
	template <typename UnitT>
	struct standard_typedefs
	{
		using value_type = UnitT;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;

		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
	};
}

#endif