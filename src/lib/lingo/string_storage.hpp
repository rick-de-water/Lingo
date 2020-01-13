#ifndef H_LINGO_STRING_STORAGE
#define H_LINGO_STRING_STORAGE

#include <lingo/platform/constexpr.hpp>
#include <lingo/utility/compressed_pair.hpp>

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
			grow_empty(storage.size());
			copy_construct(data(), storage.data(), storage.size() + 1);
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
					copy_construct(data(), storage.data(), storage.size() + 1);
					set_size(storage.size());
				}
			}
			// Allocators cannot share memory, so we have to copy
			else
			{
				grow_empty(storage.size());
				copy_construct(data(), storage.data(), storage.size() + 1);
				set_size(storage.size());
			}
		}

		~basic_string_storage() noexcept
		{
			destruct(data(), size() + 1);
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

		// Unit construction and destruction operations
		static void default_construct(pointer destination, size_type size) noexcept(noexcept(std::declval<basic_string_storage&>().default_construct_impl(destination, size)))
		{
			default_construct_impl(destination, size);
		}

		static void copy_construct(pointer destination, const_pointer source, size_type size) noexcept(noexcept(std::declval<basic_string_storage&>().copy_construct_impl(destination, source, size)))
		{
			copy_construct_impl(destination, source, size);
		}

		static void move_contruct(pointer destination, const_pointer source, size_type size) noexcept(noexcept(std::declval<basic_string_storage&>().move_construct_impl(destination, source, size)))
		{
			move_construct_impl(destination, source, size);
		}

		static void destruct(pointer destination, size_type size) noexcept(noexcept(std::declval<basic_string_storage&>().destruct_impl(destination, size)))
		{
			destruct_impl(destination, size);
		}

		// Combined operations
		// Grow when empty (usually from constructor)
		void grow_empty(size_type new_capacity)
		{
			const auto alloc = allocate(new_capacity);
			swap_data(alloc);
		}

		// Grow while discarding existing data (usually due to some kind of copy assignment)
		void grow_discard(size_type new_capacity)
		{
			const allocation current_alloc = current_allocation();
			const auto alloc = allocate(new_capacity);
			try
			{
				destruct(data(), size() + 1);
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
				move_contruct(alloc.data, current_alloc.data, size() + 1);

				// Destruct old data
				destruct(current_alloc.data, size() + 1);

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
				destruct(data() + size(), 1);
			}
			// New buffer has been allocated
			else
			{
				try
				{
					// Move all data over to the new buffer
					move_contruct(alloc.data, current_alloc.data, size());

					// Destruct old data
					destruct(current_alloc.data, size() + 1);

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

		basic_string_storage& operator = (const basic_string_storage& storage)
		{
			if (&storage != this)
			{
				grow_discard(storage.size());
				copy_construct(data(), storage.data(), storage.size() + 1);
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
							destruct(data(), size() + 1);
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

		template <typename _ = int, typename std::enable_if<
			std::is_trivially_default_constructible<value_type>::value, _>::type = 0>
		static void default_construct_impl(pointer, size_type) noexcept
		{
		}

		template <typename _ = int, typename std::enable_if<
			!std::is_trivially_default_constructible<value_type>::value &&
			std::is_default_constructible<value_type>::value, _>::type = 0>
		static void default_construct_impl(pointer destination, size_type size) noexcept(noexcept(new (destination) value_type()))
		{
			for (size_type i = 0; i < size; ++i)
			{
				new (destination + i) value_type();
			}
		}

		template <typename _ = int, typename std::enable_if<
			std::is_trivially_copy_constructible<value_type>::value, _>::type = 0>
		static void copy_construct_impl(pointer destination, const_pointer source, size_type size) noexcept
		{
			std::memcpy(destination, source, size * sizeof(unit_type));
		}

		template <typename _ = int, typename std::enable_if<
			!std::is_trivially_copy_constructible<value_type>::value &&
			std::is_copy_constructible<value_type>::value, _>::type = 0>
		static void copy_construct_impl(pointer destination, const_pointer source, size_type size) noexcept(noexcept(new (destination) value_type(source[0])))
		{
			for (size_type i = 0; i < size; ++i)
			{
				new (destination + i) value_type(source[i]);
			}
		}

		template <typename _ = int, typename std::enable_if<
			std::is_trivially_move_constructible<value_type>::value, _>::type = 0>
		static void move_construct_impl(pointer destination, const_pointer source, size_type size) noexcept
		{
			std::memcpy(destination, source, size * sizeof(unit_type));
		}

		template <typename _ = int, typename std::enable_if<
			!std::is_trivially_move_constructible<value_type>::value &&
			std::is_move_constructible<value_type>::value, _>::type = 0>
		static void move_construct_impl(pointer destination, const_pointer source, size_type size) noexcept(noexcept(new (destination) value_type(std::move(source[0]))))
		{
			for (size_type i = 0; i < size; ++i)
			{
				new (destination + i) value_type(source[i]);
			}
		}

		template <typename _ = int, typename std::enable_if<
			std::is_trivially_destructible<value_type>::value, _>::type = 0>
		static void destruct_impl(pointer, size_type) noexcept
		{
		}

		template <typename _ = int, typename std::enable_if<
			!std::is_trivially_destructible<value_type>::value &&
			std::is_destructible<value_type>::value, _>::type = 0>
		static void destruct_impl(pointer destination, size_type size) noexcept(noexcept((destination)->~value_type()))
		{
			for (size_type i = 0; i < size; ++i)
			{
				(destination + i)->~value_type();
			}
		}

		allocation allocate(size_type requested_capacity) noexcept(noexcept(std::declval<allocator_type&>().allocate(requested_capacity)))
		{
			assert(requested_capacity <= max_size());

			// Don't do anything when there is already enough capacity
			size_type new_capacity = capacity();
			if (new_capacity >= requested_capacity)
			{
				allocation alloc;
				alloc.data = data();
				alloc.size = new_capacity;
				return alloc;
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
			alloc.data = allocator.allocate(new_capacity + 1);
			alloc.size = new_capacity;
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

			const allocation original_alloc = current_allocation();

			// Free original data if it was allocated by the allocator
			if (is_long())
			{
				free(original_alloc);
			}

			// Replace the old data pointers with the new ones
			if (is_long_allocation(alloc))
			{
				auto& long_storage = _data.first()._long;
				long_storage._data = alloc.data;
				long_storage._capacity = alloc.size;
				long_storage._last_unit = internal::basic_string_storage_long_marker<value_type>::value;
			}
			else
			{
				auto& short_storage = _data.first()._short;
				short_storage._data[0];
				short_storage._last_unit = internal::basic_string_storage_short_marker<Unit>::value;
			}
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