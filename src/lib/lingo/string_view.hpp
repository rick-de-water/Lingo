#ifndef H_LINGO_STRING_VIEW
#define H_LINGO_STRING_VIEW

#include <lingo/constexpr.hpp>
#include <lingo/string_view_storage.hpp>
#include <lingo/strlen.hpp>

#include <lingo/sets/ascii.hpp>
#include <lingo/sets/unicode.hpp>

#include <lingo/encoding/none.hpp>
#include <lingo/encoding/utf8.hpp>

#include <cstddef>
#include <iterator>
#include <memory>
#include <stdexcept>

namespace lingo
{
	template <typename UnitT, typename CharacterSet, typename Encoding>
	class basic_string_view
	{
		public:
		using character_set_type = CharacterSet;
		using encoding_type = Encoding;

		using value_type = UnitT;
		using reference = UnitT&;
		using const_reference = const UnitT&;
		using pointer = UnitT*;
		using const_pointer = const UnitT*;

		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;

		using const_iterator = const_pointer;
		using iterator = const_iterator;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
		using reverse_iterator = const_reverse_iterator;

		LINGO_CONSTEXPR11 basic_string_view() noexcept = default;
		LINGO_CONSTEXPR11 basic_string_view(const basic_string_view&) noexcept = default;
		LINGO_CONSTEXPR11 basic_string_view(basic_string_view&&) noexcept = default;

		LINGO_CONSTEXPR11 basic_string_view(const_pointer string) noexcept:
			basic_string_view(string, strlen(string), true)
		{
		}

		LINGO_CONSTEXPR11 basic_string_view(const_pointer string, size_type length, bool null_terminated) noexcept:
			_storage(string, length, null_terminated)
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

		LINGO_CONSTEXPR11 iterator rbegin() const noexcept
		{
			return crbegin();
		}

		LINGO_CONSTEXPR11 iterator rend() const noexcept
		{
			return crend();
		}

		LINGO_CONSTEXPR11 const_iterator crbegin() const noexcept
		{
			return const_reverse_iterator(cend());
		}

		LINGO_CONSTEXPR11 const_iterator crend() const noexcept
		{
			return const_reverse_iterator(cbegin());
		}
		
		LINGO_CONSTEXPR11 const_reference operator [] (size_type pos) const noexcept
		{
			return data()[pos];
		}

		LINGO_CONSTEXPR11 const_reference at(size_type pos) const
		{
			if (pos < length())
			{
				return operator[](pos);
			}
			else
			{
				throw std::out_of_range();
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

		LINGO_CONSTEXPR11 size_type length() const noexcept
		{
			return size();
		}

		LINGO_CONSTEXPR11 size_type max_size() const noexcept
		{
			return _storage.max_size();
		}

		[[nodiscard]]
		LINGO_CONSTEXPR11 bool empty() const noexcept
		{
			return size() == 0;
		}

		LINGO_CONSTEXPR11 bool null_terminated() const noexcept
		{
			return _storage.null_terminated();
		}

		LINGO_CONSTEXPR11 basic_string_view& operator = (const basic_string_view&) noexcept = default;
		LINGO_CONSTEXPR11 basic_string_view& operator = (basic_string_view&&) noexcept = default;
		
		private:
		basic_string_view_storage<UnitT> _storage;
	};

	using ascii_string_view = basic_string_view<char, sets::ascii, encoding::none>;
	using utf8_string_view = basic_string_view<char, sets::unicode, encoding::utf8>;

	using string_view = utf8_string_view;
}

#endif