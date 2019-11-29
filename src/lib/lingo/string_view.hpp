#ifndef H_LINGO_STRING_VIEW
#define H_LINGO_STRING_VIEW

#include <cstddef>
#include <iterator>
#include <memory>
#include <stdexcept>

namespace lingo
{
	template <typename CharT, typename Encoding, typename CharacterSet>
	class basic_string_view
	{
		public:
		using encoding_type = Encoding;
		using character_set_type = CharacterSet;

		using value_type = CharT;
		using reference = CharT&;
		using const_reference = const CharT&;
		using pointer = CharT*;
		using const_pointer = const CharT*;

		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;

		using const_iterator = const_pointer;
		using iterator = const_iterator;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
		using reverse_iterator = const_reverse_iterator;

		constexpr basic_string_view() noexcept = default;
		constexpr basic_string_view(const basic_string_view&) noexcept = default;
		constexpr basic_string_view(basic_string_view&&) noexcept = default;

		constexpr basic_string_view(const_pointer string) noexcept:
			basic_string_view(string, strlen(string), true)
		{
		}

		constexpr basic_string_view(const_pointer string, size_type length, bool null_terminated) noexcept:
			_storage(string, length, null_terminated)
		{
		}

		constexpr iterator begin() const noexcept
		{
			return cbegin();
		}

		constexpr iterator end() const noexcept
		{
			return cend();
		}

		constexpr const_iterator cbegin() const noexcept
		{
			return const_iterator(data());
		}

		constexpr const_iterator cend() const noexcept
		{
			return const_iterator(data() + size());
		}

		constexpr iterator rbegin() const noexcept
		{
			return crbegin();
		}

		constexpr iterator rend() const noexcept
		{
			return crend();
		}

		constexpr const_iterator crbegin() const noexcept
		{
			return const_reverse_iterator(cend());
		}

		constexpr const_iterator crend() const noexcept
		{
			return const_reverse_iterator(cbegin());
		}
		
		constexpr const_reference operator [] (size_type pos) const noexcept
		{
			return data()[pos];
		}

		constexpr const_reference at(size_type pos) const
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

		constexpr const_reference front() const noexcept
		{
			return operator[](0);
		}

		constexpr const_reference back() const noexcept
		{
			return operator[](size() - 1);
		}

		constexpr const_pointer data() const noexcept
		{
			return _storage.data();
		}

		constexpr size_type size() const noexcept
		{
			return _storage.size();
		}

		constexpr size_type length() const noexcept
		{
			return size();
		}

		constexpr size_type max_size() const noexcept
		{
			return _storage.max_size();
		}

		[[nodiscard]]
		constexpr bool empty() const noexcept
		{
			return size() == 0;
		}

		constexpr bool null_terminated() const noexcept
		{
			return _storage.null_terminated();
		}


		constexpr basic_string_view& operator = (const basic_string_view&) noexcept = default;
		constexpr basic_string_view& operator = (basic_string_view&&) noexcept = default;
		
		private:
		basic_string_view_storage<CharT> _storage;
	};
}

#endif