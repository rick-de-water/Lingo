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
				_short._last_unit = Unit(((sizeof(void*) * 4) / sizeof(Unit)) - 1);
			}

			basic_string_storage_data_long<Unit> _long;
			basic_string_storage_data_short<Unit> _short;
		};

		static_assert(sizeof(basic_string_storage_data<uint_least8_t>) == sizeof(void*) * 4, "string storage is the correct size");
		static_assert(sizeof(basic_string_storage_data<uint_least16_t>) == sizeof(void*) * 4, "string storage is the correct size");
		static_assert(sizeof(basic_string_storage_data<uint_least32_t>) == sizeof(void*) * 4, "string storage is the correct size");
		static_assert(sizeof(basic_string_storage_data<uint_least64_t>) == sizeof(void*) * 4, "string storage is the correct size");
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

		basic_string_storage() noexcept(
			std::is_nothrow_constructible<allocator_type>::value &&
			std::is_nothrow_constructible<basic_string_storage, const allocator_type&>::value):
			basic_string_storage(allocator_type())
		{
		}

		explicit basic_string_storage(const allocator_type& allocator) noexcept(std::is_nothrow_copy_constructible<allocator_type>::value):
			_data{ {}, allocator }
		{
		}


		basic_string_storage(const basic_string_storage& storage, const allocator_type& allocator):
			basic_string_storage(allocator)
		{
			grow(storage.size());
			resize_copy_construct(storage.size(), storage.data());
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
					_data.first()._long._data = storage.data();
					_data.first()._long._size = storage.size();
					_data.first()._long._capacity = storage.capacity();
					_data.first()._long._last_unit = internal::basic_string_storage_long_marker<value_type>::value;

					storage._data.first()._short = {};
				}
				// Small string optimized memory cannot be moved, so we still need to copy it
				else
				{
					resize_copy_construct(storage.size(), storage.data());
				}
			}
			// Allocators cannot share memory, so we have to copy
			else
			{
				grow(storage.size());
				resize_copy_construct(storage.size(), storage.data());
			}
		}

		~basic_string_storage() noexcept
		{
			if (is_long())
			{
				destruct(data(), size());
				get_allocator().deallocate(data(), capacity());
			}
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

		void resize_default_construct(size_type new_size)
		{
			assert(new_size <= capacity());
			const size_type original_size = size();
			pointer buffer = data();
			if (new_size > original_size)
			{
				default_construct(buffer + original_size, new_size - original_size);
				buffer[new_size] = value_type{};
			}
			else if (new_size < original_size)
			{
				destruct(buffer + new_size, original_size - new_size);
				buffer[new_size] = value_type{};
			}
			set_size(new_size);
		}

		void resize_copy_construct(size_type new_size, const_pointer source)
		{
			assert(new_size <= capacity());
			const size_type original_size = size();
			pointer buffer = data();
			if (new_size > original_size)
			{
				copy_construct(buffer, source, new_size - original_size);
				buffer[new_size] = value_type{};
			}
			else if (new_size < original_size)
			{
				destruct(buffer + new_size, original_size - new_size);
				buffer[new_size] = value_type{};
			}
			set_size(new_size);
		}

		void set_size(size_type new_size)
		{
			assert(new_size <= capacity());
			if (is_long())
			{
				_data.first()._long._size = new_size;
			}
			else
			{
				_data.first()._short._last_unit = static_cast<value_type>(sizeof(void*) * 4 / sizeof(value_type) - (new_size + 1));
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

		void grow(size_type requested_capacity)
		{
			// Don't do anything when there is already enough capacity
			size_type new_capacity = capacity();
			if (new_capacity >= requested_capacity)
			{
				return;
			}

			// Calculate the new capacity
			// TODO: handle requested_capacity > max_size
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
			pointer original_data = data();
			const size_type data_size = size();
			pointer new_data = allocator.allocate(new_capacity + 1);

			// Copy the old data to the new buffer
			// TODO: rollback on exceptions when the copy constructor can throw
			copy_construct(new_data, original_data, data_size);

			// Clean up the original data
			destruct(original_data, data_size);
			
			// Free original memory if it was allocated by the allocator
			if (is_long())
			{
				allocator.deallocate(original_data, capacity());
			}

			// Replace the old data pointers with the new ones
			_data.first()._long._data = new_data;
			_data.first()._long._size = data_size;
			_data.first()._long._capacity = new_capacity;
			_data.first()._long._last_unit = internal::basic_string_storage_long_marker<value_type>::value;
		}

		basic_string_storage& operator = (const basic_string_storage& storage)
		{
			if (&storage != this)
			{
				grow(storage.size());

				resize_copy_construct(0, nullptr);
				resize_copy_construct(storage.size(), storage.data());
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
							destruct(data(), size());
							get_allocator().deallocate(data(), capacity());
						}

						// Copy memory pointers
						_data.first()._long._data = storage.data();
						_data.first()._long._size = storage.size();
						_data.first()._long._capacity = storage.capacity();
						_data.first()._long._last_unit = internal::basic_string_storage_long_marker<value_type>::value;

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

		static void default_construct(
			LINGO_UNUSED_IF_CONSTEXPR(pointer destination),
			LINGO_UNUSED_IF_CONSTEXPR(size_type size))
		{
			LINGO_IF_CONSTEXPR(!std::is_trivially_constructible<value_type>::value)
			{
				for (size_type i = 0; i < size; ++i)
				{
					new (destination + i) value_type();
				}
			}
		}

		static void copy_construct(pointer destination, const_pointer source, size_type size) noexcept(std::is_nothrow_copy_constructible<value_type>::value)
		{
			// Copy data over to the new data
			LINGO_IF_CONSTEXPR(std::is_trivially_copyable<value_type>::value)
			{
				std::memcpy(destination, source, size * sizeof(value_type));
			}
			else
			{
				for (size_type i = 0; i < size; ++i)
				{
					new (destination + i) value_type(source[i]);
				}
			}
		}

		static void destruct(
			LINGO_UNUSED_IF_CONSTEXPR(pointer destination),
			LINGO_UNUSED_IF_CONSTEXPR(size_type size))
		{
			LINGO_IF_CONSTEXPR(!std::is_trivially_destructible<value_type>::value)
			{
				for (size_type i = 0; i < size; ++i)
				{
					destination[i].~value_type();
				}
			}
		}

		private:
		utility::compressed_pair<internal::basic_string_storage_data<unit_type>, allocator_type> _data;
	};

	static_assert(sizeof(basic_string_storage<uint_least8_t>) == sizeof(void*) * 4, "string storage is the correct size");
	static_assert(sizeof(basic_string_storage<uint_least16_t>) == sizeof(void*) * 4, "string storage is the correct size");
	static_assert(sizeof(basic_string_storage<uint_least32_t>) == sizeof(void*) * 4, "string storage is the correct size");
	static_assert(sizeof(basic_string_storage<uint_least64_t>) == sizeof(void*) * 4, "string storage is the correct size");
}

#endif