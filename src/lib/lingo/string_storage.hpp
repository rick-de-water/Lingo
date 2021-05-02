#ifndef H_LINGO_STRING_STORAGE
#define H_LINGO_STRING_STORAGE

#include <lingo/platform/constexpr.hpp>

#include <lingo/utility/compressed_pair.hpp>
#include <lingo/utility/item_traits.hpp>

#include <cassert>
#include <cstddef>
#include <cstring>
#include <limits>
#include <memory>
#include <type_traits>

namespace lingo
{
	namespace internal
	{
		template <typename Unit>
		struct basic_string_storage_long_marker
		{
			static LINGO_CONSTEXPR11 Unit value = std::numeric_limits<Unit>::max();
		};

		template <typename Unit>
		struct basic_string_storage_short_marker
		{
			static LINGO_CONSTEXPR11 Unit value = Unit(((sizeof(void*) * 4) / sizeof(Unit)) - 1);
		};

		template <typename Unit>
		struct basic_string_storage_data_requires_padding
		{
			static LINGO_CONSTEXPR11 bool value = sizeof(Unit) < sizeof(void*);
		};

		template <typename Unit, bool Padding = basic_string_storage_data_requires_padding<Unit>::value>
		struct basic_string_storage_data_long;

		template <typename Unit>
		struct basic_string_storage_data_long<Unit, true>
		{
			Unit* _data;
			std::size_t _size;
			std::size_t _capacity;
			char _padding[sizeof(void*) - sizeof(Unit)];
			Unit _last_unit;
		};

		template <typename Unit>
		struct basic_string_storage_data_long<Unit, false>
		{
			Unit* _data;
			std::size_t _size;
			std::size_t _capacity;
			Unit _last_unit;
		};

		template <typename Unit>
		struct basic_string_storage_data_short
		{
			Unit _data[(sizeof(void*) * 4) / sizeof(Unit) - 1];
			Unit _last_unit;
		};

		template <typename Unit>
		union basic_string_storage_data
		{
			basic_string_storage_data() noexcept
			{
				_short._data[0] = {};
				_short._last_unit = basic_string_storage_short_marker<Unit>::value;
			}

			basic_string_storage_data_long<Unit> _long;
			basic_string_storage_data_short<Unit> _short;
		};

		static_assert(sizeof(basic_string_storage_data<uint_least8_t>) == sizeof(void*) * 4, "string storage is the correct size");
		static_assert(sizeof(basic_string_storage_data<uint_least16_t>) == sizeof(void*) * 4, "string storage is the correct size");
		static_assert(sizeof(basic_string_storage_data<uint_least32_t>) == sizeof(void*) * 4, "string storage is the correct size");
		static_assert(sizeof(basic_string_storage_data<uint_least64_t>) == sizeof(void*) * 4 || sizeof(uint_least64_t) > sizeof(void*), "string storage is the correct size");
	}

	template <typename Unit, typename Allocator = std::allocator<Unit>>
	class basic_string_storage 
	{
		public:
		using allocator_type = Allocator;

		using unit_type = Unit;

		using value_type = unit_type;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;

		using size_type = typename allocator_type::size_type;
		using difference_type = typename allocator_type::difference_type;

		static_assert(std::is_same<unit_type, typename allocator_type::value_type>::value, "allocator_type::value_type must be the same type as basic_string_storage::unit_type");

		private:
		using compressed_pair = utility::compressed_pair<internal::basic_string_storage_data<unit_type>, allocator_type>;
		using item_traits = utility::item_traits<value_type>;

		struct allocation
		{
			pointer data;
			size_type size;
		};

		public:
		basic_string_storage() noexcept(noexcept(basic_string_storage(allocator_type()))):
			basic_string_storage(allocator_type())
		{
		}

		explicit basic_string_storage(const allocator_type& allocator) noexcept(noexcept(compressed_pair{ {}, allocator })):
			_data{ {}, allocator }
		{
		}

		basic_string_storage(const basic_string_storage& storage, const allocator_type& allocator):
			basic_string_storage(allocator)
		{
			grow_discard(storage.size());
			item_traits::copy(data(), storage.data(), storage.size() + 1);
			set_size(storage.size());
		}

		basic_string_storage(basic_string_storage&& storage, const allocator_type& allocator):
			basic_string_storage(allocator)
		{
			// Move memory if it can be shared between allocators
			if (allocator == storage.get_allocator())
			{
				// Move memory if it was allocated dynamically
				if (storage.is_long())
				{
					// Copy allocation from source
					swap_data(storage.current_allocation());
					set_size(storage.size());

					// Reset source
					storage._data.first()._short = {};
				}
				// Small string optimized memory cannot be moved, so we still need to copy it
				else
				{
					// No need to grow because we know it can fit inside the storage itself
					item_traits::copy(data(), storage.data(), storage.size() + 1);
					set_size(storage.size());
				}
			}
			// Allocators cannot share memory, so we have to copy
			else
			{
				grow_discard(storage.size());
				item_traits::copy(data(), storage.data(), storage.size() + 1);
				set_size(storage.size());
			}
		}

		~basic_string_storage() noexcept
		{
			item_traits::destruct(data(), size() + 1);
			free(current_allocation());
		}

		allocator_type get_allocator() const noexcept(std::is_nothrow_copy_constructible<allocator_type>::value)
		{
			return _data.second();
		}

		pointer data() noexcept
		{
			return const_cast<pointer>(static_cast<const basic_string_storage*>(this)->data());
		}

		const_pointer data() const noexcept
		{
			if (is_long())
			{
				return _data.first()._long._data;
			}
			else
			{
				return _data.first()._short._data;
			}
		}

		size_type size() const noexcept
		{
			if (is_long())
			{
				return _data.first()._long._size;
			}
			else
			{
				return ((sizeof(void*) * 4) / sizeof(value_type)) - (static_cast<size_type>(_data.first()._short._last_unit) + 1);
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
				return _data.first()._long._capacity;
			}
			else
			{
				return (sizeof(internal::basic_string_storage_data<value_type>) / sizeof(value_type)) - 1;
			}
		}

		bool is_long() const noexcept
		{
			return _data.first()._long._last_unit == internal::basic_string_storage_long_marker<value_type>::value;
		}

		// Combined operations
		// Grow while discarding existing data (usually due to some kind of copy assignment)
		void grow_discard(size_type new_capacity)
		{
			const allocation current_alloc = current_allocation();
			const auto alloc = allocate(new_capacity);
			try
			{
				item_traits::destruct(data(), size() + 1);
				swap_data(alloc);
			}
			catch (...)
			{
				if (alloc.data != current_alloc.data)
				{
					free(alloc);
				}
				throw;
			}
		}

		// Grow while keeping the existing data including the null terminator
		void grow_reserve(size_type new_capacity)
		{
			const allocation current_alloc = current_allocation();
			const auto alloc = allocate(new_capacity);

			// Current allocation is already big enough
			if (current_alloc.data == alloc.data)
			{
				return;
			}

			// New buffer has been allocated
			try
			{
				// Move all data over to the new buffer
				item_traits::move(alloc.data, current_alloc.data, size() + 1);

				// Destruct old data
				item_traits::destruct(current_alloc.data, size() + 1);

				// Swap allocations
				swap_data(alloc);
			}
			catch (...)
			{
				if (alloc.data != current_alloc.data)
				{
					free(alloc);
				}
				throw;
			}
		}

		// Grow while keeping the existing data except for the null terminator (useful for things like string concatination)
		void grow_append(size_type new_capacity)
		{
			const allocation current_alloc = current_allocation();
			const auto alloc = allocate(new_capacity);

			// Current allocation is already big enough, so we only need to remove the null terminator
			if (current_alloc.data == alloc.data)
			{
				item_traits::destruct(data() + size(), 1);
			}
			// New buffer has been allocated
			else
			{
				try
				{
					// Move all data over to the new buffer
					item_traits::move(alloc.data, current_alloc.data, size());

					// Destruct old data
					item_traits::destruct(current_alloc.data, size() + 1);

					// Swap allocations
					swap_data(alloc);
				}
				catch (...)
				{
					if (alloc.data != current_alloc.data)
					{
						free(alloc);
					}
					throw;
				}
			}
		}

		void assign(const_pointer str, size_type length)
		{
			assert(length <= capacity());

			const pointer destination = data();

			// Copy data
			LINGO_CONSTEXPR11 const value_type null_terminator{};
			item_traits::copy(destination, str, length);
			item_traits::copy(destination + length, &null_terminator, 1);

			// Update size
			set_size(length);
		}

		void append(size_type original_size, const_pointer str, size_type length)
		{
			const size_type new_size = original_size + length;
			assert(new_size <= capacity());

			const pointer destination = data() + original_size;

			// Copy data
			LINGO_CONSTEXPR11 const value_type null_terminator{};
			item_traits::copy(destination, str, length);
			item_traits::copy(destination + length, &null_terminator, 1);

			// Update size
			set_size(new_size);
		}

		basic_string_storage& operator = (const basic_string_storage& storage)
		{
			if (&storage != this)
			{
				grow_discard(storage.size());
				item_traits::copy(data(), storage.data(), storage.size() + 1);
				set_size(storage.size());
			}
			return *this;
		}

		basic_string_storage& operator = (basic_string_storage&& storage)
		{
			if (&storage != this)
			{
				// Move memory if it can be shared between allocators
				if (get_allocator() == storage.get_allocator())
				{
					// Move memory if it was allocated dynamically
					if (storage.is_long())
					{
						// Destruct existing data if it exists
						if (is_long())
						{
							item_traits::destruct(data(), size() + 1);
						}

						// Copy allocation from source
						swap_data(storage.current_allocation());
						set_size(storage.size());

						// Mark source as empty
						storage._data.first()._short = {};
					}
					// Small string optimized memory cannot be moved, so we still need to copy it
					else
					{
						(*this) = static_cast<const basic_string_storage&>(storage);
					}
				}
				// Allocators cannot share memory, so we have to copy
				else
				{
					(*this) = static_cast<const basic_string_storage&>(storage);
				}
			}
			return *this;
		}

		allocation allocate(size_type requested_capacity) noexcept(noexcept(std::declval<allocator_type&>().allocate(requested_capacity)))
		{
			assert(requested_capacity <= max_size());

			// Don't do anything when there is already enough capacity
			size_type new_capacity = capacity();
			if (new_capacity >= requested_capacity)
			{
				return current_allocation();
			}

			// Calculate the new capacity
			if (new_capacity * 2 >= requested_capacity)
			{
				new_capacity *= 2;
			}
			else
			{
				new_capacity = requested_capacity;
			}

			// Allocate a new buffer
			allocator_type allocator = get_allocator();
			allocation alloc;
			alloc.size = new_capacity + 1;
			alloc.data = allocator.allocate(alloc.size);
			return alloc;
		}

		void free(allocation alloc) noexcept
		{
			if (is_long_allocation(alloc))
			{
				allocator_type allocator = get_allocator();
				allocator.deallocate(alloc.data, alloc.size);
			}
		}

		void swap_data(allocation alloc) noexcept
		{
			pointer original_data = data();
			if (original_data == alloc.data)
			{
				return;
			}
			assert(is_long_allocation(alloc));

			const allocation original_alloc = current_allocation();

			// Free original data if it was allocated by the allocator
			if (is_long())
			{
				free(original_alloc);
			}

			// Replace the old data pointers with the new ones
			auto& long_storage = _data.first()._long;
			long_storage._data = alloc.data;
			long_storage._capacity = alloc.size - 1;
			long_storage._last_unit = internal::basic_string_storage_long_marker<value_type>::value;
		}

		bool is_long_allocation(allocation alloc) noexcept
		{
			return alloc.data != _data.first()._short._data;
		}

		allocation current_allocation() noexcept
		{
			return allocation{ data(), capacity() + 1 };
		}

		void set_size(size_type new_size) noexcept
		{
			assert(new_size <= capacity());
			if (is_long())
			{
				_data.first()._long._size = new_size;
			}
			else
			{
				_data.first()._short._last_unit = static_cast<value_type>(internal::basic_string_storage_short_marker<Unit>::value - new_size);
			}
		}

		private:
		compressed_pair _data;
	};

	static_assert(sizeof(basic_string_storage<uint_least8_t>) == sizeof(void*) * 4, "string storage is the correct size");
	static_assert(sizeof(basic_string_storage<uint_least16_t>) == sizeof(void*) * 4, "string storage is the correct size");
	static_assert(sizeof(basic_string_storage<uint_least32_t>) == sizeof(void*) * 4, "string storage is the correct size");
	static_assert(sizeof(basic_string_storage<uint_least64_t>) == sizeof(void*) * 4 || sizeof(uint_least64_t) > sizeof(void*), "string storage is the correct size");
}

#endif