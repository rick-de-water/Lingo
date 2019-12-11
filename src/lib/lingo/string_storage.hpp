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
		union basic_string_storage_members_union
		{
			basic_string_storage_members_long<UnitT> _long;
			basic_string_storage_members_short<UnitT> _short;
		};

		template <typename UnitT>
		struct basic_string_storage_members_requires_padding
		{
			static constexpr bool value = (sizeof(basic_string_storage_members_union<UnitT>) + sizeof(UnitT)) < sizeof(void*) * 4;
		};

		template <typename UnitT, bool Padding = basic_string_storage_members_requires_padding<UnitT>::value>
		struct basic_string_storage_members;

		template <typename UnitT>
		struct basic_string_storage_members<UnitT, true>
		{
			basic_string_storage_members_union<UnitT> _specific;
			char _padding[sizeof(void*) * 4 - (sizeof(basic_string_storage_members_union<UnitT>) + sizeof(UnitT))];
			UnitT _last_char;
		};

		template <typename UnitT>
		struct basic_string_storage_members<UnitT, false>
		{
			basic_string_storage_members_union<UnitT> _specific;
			UnitT _last_char;
		};

		static_assert(sizeof(basic_string_storage_members<uint_least8_t>) == sizeof(void*) * 4, "string storage is the correct size");
		static_assert(sizeof(basic_string_storage_members<uint_least16_t>) == sizeof(void*) * 4, "string storage is the correct size");
		static_assert(sizeof(basic_string_storage_members<uint_least32_t>) == sizeof(void*) * 4, "string storage is the correct size");
		static_assert(sizeof(basic_string_storage_members<uint_least64_t>) == sizeof(void*) * 4, "string storage is the correct size");
	}

	template <typename UnitT, typename Allocator = std::allocator<UnitT>>
	class basic_string_storage : public internal::basic_string_storage_members<UnitT>
	{
		public:
		using allocator_type = Allocator;

		using value_type = UnitT;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;

		using size_type = typename allocator_type::size_type;
		using difference_type = typename allocator_type::difference_type;

		private:
		static LINGO_CONSTEXPR11 value_type long_marker = std::numeric_limits<value_type>::max();

		public:
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

	static_assert(sizeof(basic_string_storage<uint_least8_t>) == sizeof(void*) * 4, "string storage is the correct size");
	static_assert(sizeof(basic_string_storage<uint_least16_t>) == sizeof(void*) * 4, "string storage is the correct size");
	static_assert(sizeof(basic_string_storage<uint_least32_t>) == sizeof(void*) * 4, "string storage is the correct size");
	static_assert(sizeof(basic_string_storage<uint_least64_t>) == sizeof(void*) * 4, "string storage is the correct size");
}

#endif