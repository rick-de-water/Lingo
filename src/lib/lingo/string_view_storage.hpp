#ifndef H_LINGO_STRING_VIEW_STORAGE
#define H_LINGO_STRING_VIEW_STORAGE

#include <lingo/constexpr.hpp>

#include <cstddef>

namespace lingo
{
	template <typename UnitT>
	class basic_string_view_storage
	{
		public:
		using value_type = UnitT;
		using reference = UnitT&;
		using const_reference = const UnitT&;
		using pointer = UnitT*;
		using const_pointer = const UnitT*;

		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;

		static LINGO_CONSTEXPR11 size_type null_terminated_bit = 1ULL << (sizeof(size_type) * 8ULL - 1ULL);

		LINGO_CONSTEXPR11 basic_string_view_storage() noexcept = default;
		LINGO_CONSTEXPR11 basic_string_view_storage(const basic_string_view_storage&) noexcept = default;
		LINGO_CONSTEXPR11 basic_string_view_storage(basic_string_view_storage&&) noexcept = default;

		LINGO_CONSTEXPR11 basic_string_view_storage(const_pointer string, size_type length, bool null_terminated) noexcept:
			_data(string),
			_size(length | (null_terminated ? null_terminated_bit : 0))
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

		LINGO_CONSTEXPR11 basic_string_view_storage& operator = (const basic_string_view_storage&) noexcept = default;
		LINGO_CONSTEXPR11 basic_string_view_storage& operator = (basic_string_view_storage&&) noexcept = default;

		private:
		const_pointer _data = nullptr;
		size_type _size = 0;
	};
}


#endif