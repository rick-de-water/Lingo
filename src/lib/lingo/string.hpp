#ifndef H_LINGO_STRING
#define H_LINGO_STRING

#include <iterator>
#include <memory>

namespace lingo
{
	template <typename CharT, typename Encoding, typename CharacterSet, typename Allocator = std::allocator<Encoding::value_type>>
	class basic_string
	{
		public:
		using encoding_type = Encoding;
		using character_set_type = CharacterSet;
		using allocator_type = Allocator;

		using value_type = CharT;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = std::allocator_traits<allocator_type>::pointer;
		using const_pointer = std::allocator_traits<allocator_type>::const_pointer;

		using size_type = allocator_type::size_type;
		using difference_type = allocator_type::difference_type;

		using iterator = pointer;
		using const_iterator = const_pointer;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	};
}

#endif