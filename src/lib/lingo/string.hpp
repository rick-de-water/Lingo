#ifndef H_LINGO_STRING
#define H_LINGO_STRING

#include <lingo/constexpr.hpp>
#include <lingo/string_storage.hpp>

#include <lingo/set/ascii.hpp>
#include <lingo/set/unicode.hpp>

#include <lingo/encoding/none.hpp>
#include <lingo/encoding/utf8.hpp>

#include <iterator>
#include <memory>

namespace lingo
{
	template <typename UnitT, typename Encoding, typename CharacterSet, typename Allocator = std::allocator<UnitT>>
	class basic_string
	{
		public:
		using encoding_type = Encoding;
		using character_set_type = CharacterSet;
		using allocator_type = Allocator;

		using value_type = UnitT;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;

		using size_type = typename allocator_type::size_type;
		using difference_type = typename allocator_type::difference_type;

		using const_iterator = const_pointer;
		using iterator = const_iterator;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
		using reverse_iterator = const_reverse_iterator;

		private:
		using storage_type = basic_string_storage<value_type, allocator_type>;

		public:
		basic_string() noexcept = default;
	};

	using ascii_string = basic_string<char, encoding::none<char, char>, set::ascii>;
	using utf8_string = basic_string<char, encoding::utf8<char, char32_t>, set::unicode>;

	using string = utf8_string;
}

#endif