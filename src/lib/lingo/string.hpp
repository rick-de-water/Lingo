#ifndef H_LINGO_STRING
#define H_LINGO_STRING

#include <lingo/constexpr.hpp>
#include <lingo/string_converter.hpp>
#include <lingo/string_storage.hpp>
#include <lingo/string_view.hpp>

#include <lingo/encoding/cstring.hpp>
#include <lingo/encoding/none.hpp>
#include <lingo/encoding/point_iterator.hpp>
#include <lingo/encoding/utf8.hpp>
#include <lingo/encoding/utf16.hpp>

#include <lingo/page/ascii.hpp>
#include <lingo/page/cstring.hpp>
#include <lingo/page/point_mapper.hpp>
#include <lingo/page/unicode.hpp>

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

	template <typename Encoding, typename Page, typename Allocator = internal::default_allocator<Encoding>>
	class basic_string
	{
		public:
		using encoding_type = Encoding;
		using page_type = Page;
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
		static_assert(std::is_same<typename page_type::point_type, typename encoding_type::point_type>::value, "page_type::point_type must be the same type as encoding_type::point_type");
		static_assert(std::is_same<typename allocator_type::value_type, typename encoding_type::unit_type>::value, "allocator_type::value_type must be the same type as encoding_type::unit_type");

		using storage_type = basic_string_storage<value_type, allocator_type>;
		using string_view = basic_string_view<encoding_type, page_type>;

		public:
		basic_string() noexcept(noexcept(allocator_type())):
			basic_string(allocator_type())
		{
		}

		explicit basic_string(const allocator_type& allocator) noexcept:
			_storage(allocator)
		{
		}

		basic_string(size_type count, point_type point, const allocator_type& allocator = allocator_type()):
			basic_string(allocator)
		{
			// Encode the point into units
			unit_type encoded_point[encoding_type::max_units];
			const auto result = encoding_type::encode_point(point, encoded_point, encoding_type::max_units);
			if (result.error != error::error_code::success)
			{
				throw error::exception(result.error);
			}

			// Allocate memory
			_storage.grow(result.size * count);

			// Fill memory
			for (size_type i = 0; i < count; ++i)
			{
				_storage.copy_contruct(_storage.data() + i * result.size, encoded_point, result.size);
			}
			_storage.data()[result.size * count] = {};
			_storage.set_size(result.size * count);
		}

		template <std::size_t N, typename Foo = int, typename std::enable_if<(
			std::is_same<Encoding, encoding::cstring_default_encoding_t<value_type>>::value &&
			std::is_same<Page, page::cstring_default_page_t<value_type>>::value), Foo>::type = N>
		basic_string(const value_type (&cstring)[N], const allocator_type& allocator = allocator_type()):
			basic_string(string_view(cstring, N - 1, cstring[N - 1] == value_type{}), allocator)
		{
		}

		basic_string(string_view string_view, const allocator_type& allocator = allocator_type()):
			basic_string(allocator)
		{
			const size_type required_size = string_view.size();

			_storage.grow(required_size);
			_storage.resize_copy_construct(required_size, string_view.data());
		}

		template <typename SourceEncoding, typename SourcePage>
		explicit basic_string(basic_string_view<SourceEncoding, SourcePage> string_view, const allocator_type& allocator = allocator_type()):
			basic_string(string_converter<SourceEncoding, SourcePage, encoding_type, page_type>().template convert<allocator_type>(string_view, allocator))
		{
		}

		basic_string(const basic_string& string):
			basic_string(string, allocator_type())
		{
		}

		basic_string(const basic_string& string, const allocator_type& allocator):
			_storage(string._storage, allocator)
		{
		}

		basic_string(basic_string&& string):
			basic_string(std::move(string), allocator_type())
		{
		}

		basic_string(basic_string&& string, const allocator_type& allocator):
			_storage(std::move(string._storage), allocator)
		{
		}

		allocator_type get_allocator() const noexcept(std::is_nothrow_copy_constructible<allocator_type>::value)
		{
			return _storage.get_allocator();
		}

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

		void resize(size_type size) noexcept
		{
			if (size == _storage.size())
			{
				return;
			}

			if (size > _storage.capacity())
			{
				_storage.grow(size);
			}

			_storage.resize_default_construct(size);
		}

		void reserve(size_type size) noexcept
		{
			_storage.grow(size);
		}

		operator string_view() const noexcept
		{
			return string_view(data(), size(), true);
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

	// Fixed code page typedefs
	template <typename Encoding, typename Allocator = internal::default_allocator<Encoding>>
	using basic_ascii_string = basic_string<Encoding, page::ascii, Allocator>;
	template <typename Encoding, typename Allocator = internal::default_allocator<Encoding>>
	using basic_unicode_string = basic_string<Encoding, page::unicode, Allocator>;

	// Fixed encoding typedefs
	template <typename Unit, typename Allocator = internal::default_allocator<encoding::utf8<Unit, char32_t>>>
	using basic_utf8_string = basic_unicode_string<encoding::utf8<Unit, char32_t>, Allocator>;
	template <typename Unit, typename Allocator = internal::default_allocator<encoding::utf8<Unit, char32_t>>>
	using basic_utf16_string = basic_unicode_string<encoding::utf16<Unit, char32_t>, Allocator>;
	template <typename Unit, typename Allocator = internal::default_allocator<encoding::none<Unit, char32_t>>>
	using basic_utf32_string = basic_unicode_string<encoding::none<Unit, char32_t>, Allocator>;

	// Fixed encoding & code page typedefs
	template <typename Allocator = internal::default_allocator<encoding::cstring_default_encoding_t<char>>>
	using basic_narrow_string = basic_string<encoding::cstring_default_encoding_t<char>, page::cstring_default_page_t<char>, Allocator>;
	template <typename Allocator = internal::default_allocator<encoding::cstring_default_encoding_t<wchar_t>>>
	using basic_wide_string = basic_string<encoding::cstring_default_encoding_t<wchar_t>, page::cstring_default_page_t<wchar_t>, Allocator>;

	// Fully specialized typedefs
	using narrow_string = basic_narrow_string<>;
	using wide_string = basic_wide_string<>;

	using ascii_string = basic_ascii_string<encoding::none<char, char>>;

	#ifdef __cpp_char8_t
	using utf8_string = basic_utf8_string<char8_t>;
	#else
	using utf8_string = basic_utf8_string<char>;
	#endif
	using utf16_string = basic_utf8_string<char16_t>;
	using utf32_string = basic_utf32_string<char32_t>;

	// Default string typedef
	using string = utf8_string;
}

#endif