#ifndef H_LINGO_STRING_STORAGE
#define H_LINGO_STRING_STORAGE

#include <lingo/constexpr.hpp>

#include <cstddef>
#include <limits>
#include <memory>
#include <type_traits>

namespace lingo
{
	namespace internal
	{
		template <typename UnitT>
		struct basic_string_storage_members_long
		{
			UnitT* _data;
			std::size_t _size;
			std::size_t _capacity;
		};

		template <typename UnitT>
		struct basic_string_storage_members_short
		{
			UnitT _data[(sizeof(UnitT*) * 3) / sizeof(UnitT)];
		};

		template <typename UnitT>
		struct basic_string_storage_members
		{
			union
			{
				basic_string_storage_members_long<UnitT> _long;
				basic_string_storage_members_short<UnitT> _short;
			} _specific;

			UnitT _padding[(sizeof(UnitT*) / sizeof(UnitT)) - 1];
			UnitT _last_char;
		};

		static_assert(sizeof(basic_string_storage_members<char>) == sizeof(void*) * 4, "string storage is the correct size");
		static_assert(sizeof(basic_string_storage_members<char16_t>) == sizeof(void*) * 4, "string storage is the correct size");
		static_assert(sizeof(basic_string_storage_members<char32_t>) == sizeof(void*) * 4, "string storage is the correct size");
		static_assert(sizeof(basic_string_storage_members<long long>) == sizeof(void*) * 4, "string storage is the correct size");
	}

	template <typename UnitT, typename Allocator = std::allocator<UnitT>>
	class basic_string_storage : private internal::basic_string_storage_members<UnitT>
	{
		public:
		using value_type = UnitT;
		using reference = UnitT&;
		using const_reference = const UnitT&;
		using pointer = UnitT*;
		using const_pointer = const UnitT*;

		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;

		static LINGO_CONSTEXPR11 value_type long_marker = std::numeric_limits<value_type>::max();

		basic_string_storage() noexcept = default;

		pointer data() const noexcept
		{
			if (is_long())
			{
				return _specific._long._data;
			}
			else
			{
				return _specific._short._data;
			}
		}

		size_type size() const noexcept
		{
			if (is_long())
			{
				return _specific._long._size;
			}
			else
			{
				return sizeof(internal::basic_string_storage_members_short<value_type>) - _last_char;
			}
		}

		size_type max_size() const noexcept
		{
			return std::numeric_limits<size_t>::max();
		}

		size_type capacity() const noexcept
		{
			if (is_long())
			{
				return _specific._long._capacity;
			}
			else
			{
				return sizeof(internal::basic_string_storage_members_short<value_type>);
			}
		}

		bool is_long() const noexcept
		{
			return _last_char == long_marker;
		}
	};

	static_assert(sizeof(basic_string_storage<char>) == sizeof(void*) * 4, "string storage is the correct size");
	static_assert(sizeof(basic_string_storage<char16_t>) == sizeof(void*) * 4, "string storage is the correct size");
	static_assert(sizeof(basic_string_storage<char32_t>) == sizeof(void*) * 4, "string storage is the correct size");
	static_assert(sizeof(basic_string_storage<long long>) == sizeof(void*) * 4, "string storage is the correct size");
}

#endif