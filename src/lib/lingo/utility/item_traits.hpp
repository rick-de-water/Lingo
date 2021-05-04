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
			struct construct_items_impl;

			template <typename Item>
			struct construct_items_impl<Item, typename std::enable_if<
				std::is_trivially_default_constructible<Item>::value>::type>
			{
				void operator () (Item*, size_t) noexcept {};
			};

			template <typename Item>
			struct construct_items_impl<Item, typename std::enable_if<
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
			struct copy_items_impl;

			template <typename Item>
			struct copy_items_impl<Item, typename std::enable_if<
				std::is_trivially_copy_constructible<Item>::value>::type>
			{
				void operator () (Item* destination, const Item* source, size_t size) noexcept
				{
					std::memcpy(destination, source, size * sizeof(Item));
				}
			};

			template <typename Item>
			struct copy_items_impl<Item, typename std::enable_if<
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
			struct move_items_impl;

			template <typename Item>
			struct move_items_impl<Item, typename std::enable_if<
				std::is_trivially_move_constructible<Item>::value>::type>
			{
				void operator () (Item* destination, Item* source, size_t size) noexcept
				{
					std::memcpy(destination, source, size * sizeof(Item));
				}
			};

			template <typename Item>
			struct move_items_impl<Item, typename std::enable_if<
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
			struct destruct_items_impl;

			template <typename Item>
			struct destruct_items_impl<Item, typename std::enable_if<
				std::is_trivially_destructible<Item>::value>::type>
			{
				void operator () (Item*, size_t) noexcept {};
			};

			template <typename Item>
			struct destruct_items_impl<Item, typename std::enable_if<
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
		struct construct_items : internal::construct_items_impl<Item> {};

		template <typename Item>
		struct destruct_items : internal::destruct_items_impl<Item> {};

		template <typename Item>
		struct copy_items : internal::copy_items_impl<Item> {};

		template <typename Item>
		struct move_items : internal::move_items_impl<Item> {};

		template <typename Item>
		struct destructive_move_items
		{
			void operator () (Item* destination, Item* source, size_t size) noexcept(
				noexcept(std::declval<move_items<Item>&>()(destination, source, size)) &&
				noexcept(std::declval<destruct_items<Item>&>()(source, size)))
			{
				move_items<Item>{}(destination, source, size);
				destruct_items<Item>{}(source, size);
			}
		};
    }
}

#endif