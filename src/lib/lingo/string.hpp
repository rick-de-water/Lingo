#ifndef H_LINGO_STRING
#define H_LINGO_STRING

#include <lingo/constexpr.hpp>

#include <lingo/set/ascii.hpp>
#include <lingo/set/unicode.hpp>

#include <lingo/encoding/none.hpp>
#include <lingo/encoding/utf8.hpp>

#include <iterator>
#include <memory>

namespace lingo
{
	template <typename UnitT, typename CharacterSet, typename Encoding, typename Allocator = std::allocator<UnitT>>
	class basic_string
	{
		public:
		using encoding_type = Encoding;
		using character_set_type = CharacterSet;
		using allocator_type = Allocator;

		using value_type = UnitT;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = typename std::allocator_traits<allocator_type>::pointer;
		using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;

		using size_type = typename allocator_type::size_type;
		using difference_type = typename allocator_type::difference_type;

		using iterator = pointer;
		using const_iterator = const_pointer;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	};

	using ascii_string = basic_string<char, set::ascii, encoding::none>;
	using utf8_string = basic_string<char, set::unicode, encoding::utf8>;

	using string = utf8_string;
}

#endif