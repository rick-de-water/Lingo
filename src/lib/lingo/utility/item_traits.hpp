#ifndef H_LINGO_ITEM_TRAITS
#define H_LINGO_ITEM_TRAITS

#include <cstddef>
#include <cstring>
#include <type_traits>

namespace lingo
{
    namespace utility
    {
        namespace internal
        {
			// Default
			template <typename Item, typename = void>
			struct construct_impl;

			template <typename Item>
			struct construct_impl<Item, typename std::enable_if<
				std::is_trivially_default_constructible<Item>::value>::type>
			{
				void operator () (Item*, size_t) noexcept {};
			};

			template <typename Item>
			struct construct_impl<Item, typename std::enable_if<
				!std::is_trivially_default_constructible<Item>::value &&
				std::is_default_constructible<Item>::value>::type>
			{
				void operator () (Item* destination, size_t size) noexcept(noexcept(new (destination) Item()))
				{
					for (size_t i = 0; i < size; ++i)
					{
						new (destination + i) Item();
					}
				}
			};

			// Copy
			template <typename Item, typename = void>
			struct copy_impl;

			template <typename Item>
			struct copy_impl<Item, typename std::enable_if<
				std::is_trivially_copy_constructible<Item>::value>::type>
			{
				void operator () (Item* destination, const Item* source, size_t size) noexcept
				{
					std::memcpy(destination, source, size * sizeof(Item));
				}
			};

			template <typename Item>
			struct copy_impl<Item, typename std::enable_if<
				!std::is_trivially_copy_constructible<Item>::value &&
				std::is_copy_constructible<Item>::value>::type>
			{
				void operator () (Item* destination, const Item* source, size_t size) noexcept(noexcept(new (destination) Item()))
				{
					for (size_t i = 0; i < size; ++i)
					{
						new (destination + i) Item(source[i]);
					}
				}
			};

			// Move
			template <typename Item, typename = void>
			struct move_impl;

			template <typename Item>
			struct move_impl<Item, typename std::enable_if<
				std::is_trivially_move_constructible<Item>::value>::type>
			{
				void operator () (Item* destination, Item* source, size_t size) noexcept
				{
					std::memcpy(destination, source, size * sizeof(Item));
				}
			};

			template <typename Item>
			struct move_impl<Item, typename std::enable_if<
				!std::is_trivially_move_constructible<Item>::value &&
				std::is_move_constructible<Item>::value>::type>
			{
				void operator () (Item* destination, Item* source, size_t size) noexcept(noexcept(new (destination) Item()))
				{
					for (size_t i = 0; i < size; ++i)
					{
						new (destination + i) Item(std::move(source[i]));
					}
				}
			};

			// destruct
			template <typename Item, typename = void>
			struct destruct_impl;

			template <typename Item>
			struct destruct_impl<Item, typename std::enable_if<
				std::is_trivially_destructible<Item>::value>::type>
			{
				void operator () (Item*, size_t) noexcept {};
			};

			template <typename Item>
			struct destruct_impl<Item, typename std::enable_if<
				!std::is_trivially_destructible<Item>::value &&
				std::is_destructible<Item>::value>::type>
			{
				void operator () (Item* destination, size_t size) noexcept(noexcept(new (destination) Item()))
				{
					for (size_t i = 0; i < size; ++i)
					{
						(destination + i)->~Item();
					}
				}
			};
        }

        template <typename Item>
        struct item_traits
        {
			using value_type = Item;
			using reference = value_type&;
			using const_reference = const value_type&;
			using pointer = value_type*;
			using const_pointer = const value_type*;

			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;

			static void construct(pointer destination, size_type size) noexcept(noexcept(std::declval<internal::construct_impl<value_type>>()(destination, size)))
			{
				internal::construct_impl<Item>{}(destination, size);
			}
			
			static void destruct(pointer destination, size_type size) noexcept(noexcept(std::declval<internal::destruct_impl<value_type>>()(destination, size)))
			{
				internal::destruct_impl<Item>{}(destination, size);
			}

			static void copy(pointer destination, const_pointer source, size_type size) noexcept(noexcept(std::declval<internal::copy_impl<value_type>&>()(destination, source, size)))
			{
				internal::copy_impl<Item>{}(destination, source, size);
			}

			static void move(pointer destination, pointer source, size_type size) noexcept(noexcept(std::declval<internal::move_impl<value_type>&>()(destination, source, size)))
			{
				internal::move_impl<Item>{}(destination, source, size);
			}

			static void destructive_move(pointer destination, pointer source, size_type size) noexcept(noexcept(move(destination, source, size)) && noexcept(destruct(source, size)))
			{
				move(destination, source, size);
				destruct(source, size);
			}
        };
    }
}

#endif