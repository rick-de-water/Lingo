#ifndef H_LINGO_UTILITY_OBJECT_BUILDER
#define H_LINGO_UTILITY_OBJECT_BUILDER

#include <lingo/platform/constexpr.hpp>

#include <cstddef>
#include <memory>
#include <cstring>
#include <type_traits>

namespace lingo
{
	namespace utility
	{
		template <typename Object>
		struct object_builder
		{
			public:
			using value_type = Object;
			using reference = value_type&;
			using const_reference = const value_type&;
			using pointer = value_type*;
			using const_pointer = const value_type*;

			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;

			static void default_construct(pointer destination, size_type size) noexcept(noexcept(std::declval<object_builder&>().default_construct_impl(destination, size)))
			{
				default_construct_impl(destination, size);
			}

			static void copy_construct(pointer destination, const_pointer source, size_type size) noexcept(noexcept(std::declval<object_builder&>().copy_construct_impl(destination, source, size)))
			{
				copy_construct_impl(destination, source, size);
			}

			static void move_contruct(pointer destination, const_pointer source, size_type size) noexcept(noexcept(std::declval<object_builder&>().move_construct_impl(destination, source, size)))
			{
				move_construct_impl(destination, source, size);
			}

			static void destruct(pointer destination, size_type size) noexcept(noexcept(std::declval<object_builder&>().destruct_impl(destination, size)))
			{
				destruct_impl(destination, size);
			}

			private:
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
				std::memcpy(destination, source, size * sizeof(value_type));
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
				std::memcpy(destination, source, size * sizeof(value_type));
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
		};
	}
}

#endif