#ifndef H_LINGO_STRING
#define H_LINGO_STRING

#include <lingo/constexpr.hpp>
#include <lingo/string_storage.hpp>
#include <lingo/string_view.hpp>

#include <lingo/set/ascii.hpp>
#include <lingo/set/unicode.hpp>

#include <lingo/encoding/none.hpp>
#include <lingo/encoding/utf8.hpp>

#include <cassert>
#include <iterator>
#include <memory>
#include <type_traits>
#include <string>

namespace lingo
{
	namespace internal
	{
		template <typename Encoding>
		using default_allocator = std::allocator<typename Encoding::unit_type>;
	}

	template <typename Encoding, typename CharacterSet, typename Allocator = internal::default_allocator<Encoding>>
	class basic_string
	{
		public:
		using encoding_type = Encoding;
		using character_set_type = CharacterSet;
		using allocator_type = Allocator;

		using unit_type = typename encoding_type::unit_type;
		using point_type = typename encoding_type::point_type;

		using value_type = unit_type;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = typename std::allocator_traits<allocator_type>::pointer;
		using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;

		using size_type = typename std::allocator_traits<allocator_type>::size_type;
		using difference_type = typename std::allocator_traits<allocator_type>::difference_type;

		using iterator = pointer;
		using const_iterator = const_pointer;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		private:
		static_assert(std::is_same<typename character_set_type::point_type, typename encoding_type::point_type>::value, "character_set_type::point_type must be the same type as encoding_type::point_type");
		static_assert(std::is_same<typename allocator_type::value_type, typename encoding_type::unit_type>::value, "allocator_type::value_type must be the same type as encoding_type::unit_type");

		using storage_type = basic_string_storage<value_type, allocator_type>;
		using string_view = basic_string_view<encoding_type, character_set_type>;

		public:
		basic_string() noexcept(noexcept(allocator_type())):
			basic_string(allocator_type())
		{
		}

		explicit basic_string(const allocator_type& allocator) noexcept:
			_storage(allocator)
		{
		}

		basic_string(const_pointer cstring, const allocator_type& allocator = allocator_type()):
			basic_string(string_view(cstring), allocator)
		{
		}

		basic_string(const_pointer cstring, size_type count, const allocator_type& allocator = allocator_type()):
			basic_string(string_view(cstring, count), allocator)
		{
		}

		basic_string(string_view string_view, const allocator_type& allocator = allocator_type()):
			_storage(allocator)
		{
			const size_type required_size = string_view.size();

			_storage.grow(required_size);
			_storage.resize_copy_construct(required_size, string_view.data());
		}

		/*basic_string(const_pointer cstring):
			basic_string(allocator)
		{
		}

		/*template <typename T, typename std::enable_if<std::is_convertible<const T&, string_view>::value, int>::type = 0>
		basic_string(const T& value, const allocator_type& allocator = allocator_type()):
			basic_string(allocator)
		{
		}*/

		iterator begin() noexcept
		{
			return iterator(data());
		}

		iterator end() noexcept
		{
			return iterator(data() + size());
		}

		const_iterator cbegin() const noexcept
		{
			return const_iterator(data());
		}

		const_iterator cend() const noexcept
		{
			return const_iterator(data() + size());
		}

		reverse_iterator rbegin() noexcept
		{
			return reverse_iterator(begin());
		}

		reverse_iterator rend() noexcept
		{
			return reverse_iterator(end());
		}

		const_reverse_iterator crbegin() const noexcept
		{
			return const_reverse_iterator(cend());
		}

		const_reverse_iterator crend() const noexcept
		{
			return const_reverse_iterator(cbegin());
		}

		reference operator [] (size_type pos) noexcept
		{
			return const_cast<reference>(static_cast<const basic_string*>(this)->operator[](pos));
		}

		const_reference operator [] (size_type pos) const noexcept
		{
			return data()[pos];
		}

		reference at(size_type pos)
		{
			return const_cast<reference>(static_cast<const basic_string*>(this)->at(pos));
		}

		const_reference at(size_type pos) const
		{
			if (pos < size())
			{
				return operator[](pos);
			}
			else
			{
				throw std::out_of_range("Index out of range");
			}
		}

		reference front() noexcept
		{
			return const_cast<reference>(static_cast<const basic_string*>(this)->front());
		}

		const_reference front() const noexcept
		{
			return operator[](0);
		}

		reference back() noexcept
		{
			return const_cast<reference>(static_cast<const basic_string*>(this)->back());
		}

		const_reference back() const noexcept
		{
			return operator[](size() - 1);
		}

		pointer data() noexcept
		{
			return const_cast<pointer>(static_cast<const basic_string*>(this)->data());
		}

		const_pointer data() const noexcept
		{
			return _storage.data();
		}

		size_type size() const noexcept
		{
			return _storage.size();
		}

		size_type max_size() const noexcept
		{
			return _storage.max_size();
		}

		bool empty() const noexcept
		{
			return size() == 0;
		}

		const_pointer c_str() const noexcept
		{
			return data();
		}

		template <typename Traits = std::char_traits<value_type>, typename StdAllocator = allocator_type>
		std::basic_string<value_type, Traits, StdAllocator> std() const noexcept
		{
			return std::basic_string<value_type, Traits, StdAllocator>(data(), size());
		}

		private:
		storage_type _storage;
	};

	template <typename Encoding, typename Allocator = internal::default_allocator<Encoding>>
	using basic_ascii_string = basic_string<Encoding, set::ascii, Allocator>;
	template <typename Encoding, typename Allocator = internal::default_allocator<Encoding>>
	using basic_unicode_string = basic_string<Encoding, set::unicode<char32_t>, Allocator>;

	template <typename UnitT, typename Allocator = internal::default_allocator<encoding::utf8<UnitT, char32_t>>>
	using basic_utf8_string = basic_unicode_string<encoding::utf8<UnitT, char32_t>, Allocator>;
	template <typename UnitT, typename Allocator = internal::default_allocator<encoding::none<UnitT, char32_t>>>
	using basic_utf32_string = basic_unicode_string<encoding::none<UnitT, char32_t>, Allocator>;

	using utf8_string = basic_utf8_string<char>;
	using utf32_string = basic_utf32_string<char32_t>;

	using string = utf8_string;
}

#endif