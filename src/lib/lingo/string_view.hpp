#ifndef H_LINGO_STRING_VIEW
#define H_LINGO_STRING_VIEW

#include <lingo/platform/constexpr.hpp>
#include <lingo/string_view_storage.hpp>
#include <lingo/strlen.hpp>

#include <lingo/page/ascii.hpp>
#include <lingo/page/cstring.hpp>
#include <lingo/page/unicode.hpp>

#include <lingo/encoding/cstring.hpp>
#include <lingo/encoding/none.hpp>
#include <lingo/encoding/utf8.hpp>
#include <lingo/encoding/utf16.hpp>
#include <lingo/encoding/utf32.hpp>

#include <cstddef>
#include <iterator>
#include <memory>
#include <stdexcept>

#ifdef __cpp_lib_string_view
#include <string_view>
#endif

namespace lingo
{
	template <typename Encoding, typename Page>
	class basic_string_view
	{
		public:
		using encoding_type = Encoding;
		using page_type = Page;

		using unit_type = typename encoding_type::unit_type;
		using point_type = typename encoding_type::point_type;

		using value_type = unit_type;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;

		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;

		using const_iterator = const_pointer;
		using iterator = const_iterator;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
		using reverse_iterator = const_reverse_iterator;

		private:
		static_assert(std::is_same<typename page_type::point_type, typename encoding_type::point_type>::value, "page_type::point_type must be the same type as encoding_type::point_type");

		using storage_type = basic_string_view_storage<value_type>;

		public:
		LINGO_CONSTEXPR11 basic_string_view() noexcept = default;
		LINGO_CONSTEXPR11 basic_string_view(const basic_string_view&) noexcept = default;
		LINGO_CONSTEXPR11 basic_string_view(basic_string_view&&) noexcept = default;

		template <std::size_t N, typename Foo = int, typename std::enable_if<(
			std::is_same<Encoding, encoding::cstring_default_encoding_t<value_type>>::value &&
			std::is_same<Page, page::cstring_default_page_t<value_type>>::value), Foo>::type = N>
		LINGO_CONSTEXPR11 basic_string_view(const value_type (&cstring)[N]) noexcept:
			basic_string_view(cstring, N - 1, cstring[N - 1] == value_type{})
		{
		}

		template <std::size_t N, typename Foo = int, typename std::enable_if<!(
			std::is_same<Encoding, encoding::cstring_default_encoding_t<value_type>>::value &&
			std::is_same<Page, page::cstring_default_page_t<value_type>>::value), Foo>::type = N>
		explicit LINGO_CONSTEXPR11 basic_string_view(const value_type (&cstring)[N]) noexcept:
			basic_string_view(cstring, N - 1, cstring[N - 1] == value_type{})
		{
		}

		explicit LINGO_CONSTEXPR11 basic_string_view(const_pointer cstring) noexcept:
			basic_string_view(cstring, strlen(cstring), true)
		{
		}

		LINGO_CONSTEXPR11 basic_string_view(const_pointer cstring, size_type size) noexcept:
			basic_string_view(cstring, size, false)
		{
		}

		LINGO_CONSTEXPR11 basic_string_view(const_pointer cstring, size_type size, bool null_terminated) noexcept:
			_storage(cstring, size, null_terminated)
		{
		}

		LINGO_CONSTEXPR11 iterator begin() const noexcept
		{
			return cbegin();
		}

		LINGO_CONSTEXPR11 iterator end() const noexcept
		{
			return cend();
		}

		LINGO_CONSTEXPR11 const_iterator cbegin() const noexcept
		{
			return const_iterator(data());
		}

		LINGO_CONSTEXPR11 const_iterator cend() const noexcept
		{
			return const_iterator(data() + size());
		}

		LINGO_CONSTEXPR11 reverse_iterator rbegin() const noexcept
		{
			return crbegin();
		}

		LINGO_CONSTEXPR11 reverse_iterator rend() const noexcept
		{
			return crend();
		}

		LINGO_CONSTEXPR11 const_reverse_iterator crbegin() const noexcept
		{
			return const_reverse_iterator(cend());
		}

		LINGO_CONSTEXPR11 const_reverse_iterator crend() const noexcept
		{
			return const_reverse_iterator(cbegin());
		}
		
		LINGO_CONSTEXPR11 const_reference operator [] (size_type pos) const noexcept
		{
			return data()[pos];
		}

		LINGO_CONSTEXPR14 const_reference at(size_type pos) const
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

		LINGO_CONSTEXPR11 const_reference front() const noexcept
		{
			return operator[](0);
		}

		LINGO_CONSTEXPR11 const_reference back() const noexcept
		{
			return operator[](size() - 1);
		}

		LINGO_CONSTEXPR11 const_pointer data() const noexcept
		{
			return _storage.data();
		}

		LINGO_CONSTEXPR11 size_type size() const noexcept
		{
			return _storage.size();
		}

		LINGO_CONSTEXPR11 size_type max_size() const noexcept
		{
			return _storage.max_size();
		}

		LINGO_CONSTEXPR11 bool empty() const noexcept
		{
			return size() == 0;
		}

		LINGO_CONSTEXPR11 bool null_terminated() const noexcept
		{
			return _storage.null_terminated();
		}

		#ifdef __cpp_lib_string_view
		template <typename Traits = std::char_traits<value_type>>
		LINGO_CONSTEXPR17 std::basic_string_view<value_type, Traits> std() const noexcept
		{
			return std::basic_string_view<value_type, Traits>(data(), size());
		}
		#endif

		LINGO_CONSTEXPR14 basic_string_view remove_prefix(size_type n) noexcept
		{
			assert(n <= size());
			if (n == 0)
			{
				return basic_string_view(data(), 0, false);
			}
			else
			{
				basic_string_view removed(data(), n, false);
				_storage = storage_type(data() + n, size() - n, null_terminated());
				return removed;
			}
		}

		LINGO_CONSTEXPR14 basic_string_view remove_suffix(size_type n) noexcept
		{
			assert(n <= size());
			if (n == 0)
			{
				return basic_string_view(data() + size(), 0, null_terminated());
			}
			else
			{
				basic_string_view removed(data() + (size() - n), n, null_terminated());
				_storage = storage_type(data(), size() - n, false);
				return removed;
			}
		}

		LINGO_CONSTEXPR14 void swap(basic_string_view& other) noexcept
		{
			_storage.swap(other._storage);
		}

		LINGO_CONSTEXPR14 basic_string_view& operator = (const basic_string_view&) noexcept = default;
		LINGO_CONSTEXPR14 basic_string_view& operator = (basic_string_view&&) noexcept = default;
		
		private:
		storage_type _storage;
	};

	template <typename Encoding, typename Page>
	LINGO_CONSTEXPR14 bool operator == (basic_string_view<Encoding, Page> left, basic_string_view<Encoding, Page> right)
	{
		using size_type = typename basic_string_view<Encoding, Page>::size_type;

		if (left.size() != right.size())
		{
			return false;
		}

		for (size_type i = 0; i < left.size(); ++i)
		{
			if (left[i] != right[i])
			{
				return false;
			}
		}

		return true;
	}

	template <typename Encoding, typename Page>
	LINGO_CONSTEXPR14 bool operator != (basic_string_view<Encoding, Page> left, basic_string_view<Encoding, Page> right)
	{
		return !(left == right);
	}

	// Fixed code page typedefs
	template <typename Encoding>
	using basic_ascii_string_view = basic_string_view<Encoding, page::ascii>;
	template <typename Encoding>
	using basic_unicode_string_view = basic_string_view<Encoding, page::unicode>;

	// Fixed encoding typedefs
	template <typename Unit>
	using basic_utf8_string_view = basic_unicode_string_view<encoding::utf8<Unit, char32_t>>;
	template <typename Unit>
	using basic_utf16_string_view = basic_unicode_string_view<encoding::utf16<Unit, char32_t>>;
	template <typename Unit>
	using basic_utf32_string_view = basic_unicode_string_view<encoding::utf32<Unit, char32_t>>;

	// Fully specialized typedefs
	using narrow_string_view = basic_string_view<encoding::cstring_default_encoding_t<char>, page::cstring_default_page_t<char>>;
	using wide_string_view = basic_string_view<encoding::cstring_default_encoding_t<wchar_t>, page::cstring_default_page_t<wchar_t>>;

	using ascii_string_view = basic_ascii_string_view<encoding::none<char, char>>;

	#ifdef __cpp_char8_t
	using utf8_string_view = basic_utf8_string_view<char8_t>;
	#else
	using utf8_string_view = basic_utf8_string_view<char>;
	#endif
	using utf16_string_view = basic_utf16_string_view<char16_t>;
	using utf32_string_view = basic_utf32_string_view<char32_t>;

	// Default string_view typedef
	using string_view = utf8_string_view;
}

#endif