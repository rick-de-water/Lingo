#ifndef H_LINGO_STRING_VIEW_STORAGE
#define H_LINGO_STRING_VIEW_STORAGE

#include <lingo/platform/constexpr.hpp>

#include <cstddef>

namespace lingo
{
	template <typename Unit>
	class basic_string_view_storage
	{
		public:
		using value_type = Unit;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;

		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;

		static LINGO_CONSTEXPR11 size_type null_terminated_bit = 1ULL << (sizeof(size_type) * 8ULL - 1ULL);
		static LINGO_CONSTEXPR11 value_type null_terminator = value_type{};

		LINGO_CONSTEXPR11 basic_string_view_storage() noexcept = default;
		LINGO_CONSTEXPR11 basic_string_view_storage(const basic_string_view_storage&) noexcept = default;
		LINGO_CONSTEXPR11 basic_string_view_storage(basic_string_view_storage&&) noexcept = default;

		LINGO_CONSTEXPR11 basic_string_view_storage(const_pointer cstring, size_type size, bool null_terminated) noexcept:
			_data(cstring),
			_size(size | (null_terminated ? null_terminated_bit : 0))
		{
		}

		LINGO_CONSTEXPR11 const_pointer data() const noexcept
		{
			return _data;
		}

		LINGO_CONSTEXPR11 size_type size() const noexcept
		{
			return _size & (~null_terminated_bit);
		}

		LINGO_CONSTEXPR11 size_type max_size() const noexcept
		{
			return ~null_terminated_bit;
		}

		LINGO_CONSTEXPR11 bool null_terminated() const noexcept
		{
			return _size & null_terminated_bit;
		}

		LINGO_CONSTEXPR14 void swap(basic_string_view_storage& other) noexcept
		{
			const_pointer tmp_data = _data;
			_data = other._data;
			other._data = tmp_data;

			size_type tmp_size = _size;
			_size = other._size;
			other._size = tmp_size;
		}

		LINGO_CONSTEXPR14 basic_string_view_storage& operator = (const basic_string_view_storage&) noexcept = default;
		LINGO_CONSTEXPR14 basic_string_view_storage& operator = (basic_string_view_storage&&) noexcept = default;

		private:
		const_pointer _data = &null_terminator;
		size_type _size = null_terminated_bit;
	};

	template <typename Unit>
	LINGO_CONSTEXPR11 Unit basic_string_view_storage<Unit>::null_terminator;
}


#endif