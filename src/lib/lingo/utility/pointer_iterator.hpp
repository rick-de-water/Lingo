#ifndef H_LINGO_UTILITY_POINTER_ITERATOR
#define H_LINGO_UTILITY_POINTER_ITERATOR

#include <lingo/platform/constexpr.hpp>
#include <lingo/platform/warnings.hpp>

#include <cstddef>
#include <iterator>
#include <type_traits>

namespace lingo
{
	namespace utility
	{
		template <typename T, typename Tag>
		class pointer_iterator
		{
			public:
			#ifdef __cpp_lib_ranges
			using iterator_category = std::contiguous_iterator_tag;
			#else
			using iterator_category = std::random_access_iterator_tag;
			#endif
			using value_type = T;
			using difference_type = std::ptrdiff_t;
			using pointer = T*;
			using reference = T&;

			LINGO_CONSTEXPR11 pointer_iterator() noexcept = default;
			LINGO_CONSTEXPR11 explicit pointer_iterator(pointer value) noexcept:
				_value(value)
			{
			}

			LINGO_CONSTEXPR11 reference operator * () const noexcept
			{
				return *_value;
			}

			LINGO_CONSTEXPR11 pointer operator -> () const noexcept
			{
				return _value;
			}

			LINGO_CONSTEXPR11 pointer get() const noexcept
			{
				return _value;
			}

			LINGO_CONSTEXPR14 reference operator [] (difference_type index) const noexcept
			{
				return _value[index];
			}

			LINGO_CONSTEXPR14 pointer_iterator& operator ++ () noexcept
			{
				++_value;
				return *this;
			}

			LINGO_CONSTEXPR14 pointer_iterator operator ++ (int) noexcept
			{
				pointer_iterator original = *this;
				++_value;
				return original;
			}

			LINGO_CONSTEXPR14 pointer_iterator& operator -- () noexcept
			{
				--_value;
				return *this;
			}

			LINGO_CONSTEXPR14 pointer_iterator operator -- (int) noexcept
			{
				pointer_iterator original = *this;
				--_value;
				return original;
			}

			LINGO_CONSTEXPR14 pointer_iterator& operator += (difference_type offset) noexcept
			{
				_value += offset;
				return *this;
			}

			LINGO_CONSTEXPR14 pointer_iterator& operator -= (difference_type offset) noexcept
			{
				_value -= offset;
				return *this;
			}

			LINGO_WARNINGS_PUSH_AND_DISABLE_CLANG(everything) // Clang warns about an unused function that is actually SFINAEd away
			template <typename _ = int, typename std::enable_if<!std::is_const<value_type>::value, _>::type = 0>
			LINGO_CONSTEXPR11 operator pointer_iterator<const value_type, Tag>() const noexcept
			{
				return pointer_iterator<const value_type, Tag>(_value);
			}
			LINGO_WARNINGS_POP_CLANG

			private:
			pointer _value = nullptr;
		};

		template <typename T, typename Tag>
		LINGO_CONSTEXPR14 pointer_iterator<T, Tag> operator + (pointer_iterator<T, Tag> iterator, typename pointer_iterator<T, Tag>::difference_type offset)
		{
			return iterator += offset;
		}

		template <typename T, typename Tag>
		LINGO_CONSTEXPR14 pointer_iterator<T, Tag> operator + (typename pointer_iterator<T, Tag>::difference_type offset, pointer_iterator<T, Tag> iterator)
		{
			return iterator += offset;
		}

		template <typename T, typename Tag>
		LINGO_CONSTEXPR14 pointer_iterator<T, Tag> operator - (pointer_iterator<T, Tag> iterator, typename pointer_iterator<T, Tag>::difference_type offset)
		{
			return iterator -= offset;
		}
		
		template <typename T, typename U, typename Tag,
			typename std::enable_if<
				std::is_same<
					typename std::remove_const<T>::type,
					typename std::remove_const<U>::type
				>::value,
			int>::type = 0>
		LINGO_CONSTEXPR14 typename pointer_iterator<T, Tag>::difference_type operator - (const pointer_iterator<T, Tag>& left, const pointer_iterator<U, Tag>& right)
		{
			return static_cast<typename pointer_iterator<T, Tag>::difference_type>(left.get() - right.get());
		}
		
		template <typename T, typename U, typename Tag,
			typename std::enable_if<
				std::is_same<
					typename std::remove_const<T>::type,
					typename std::remove_const<U>::type
				>::value,
			int>::type = 0>
		LINGO_CONSTEXPR11 bool operator == (const pointer_iterator<T, Tag>& left, const pointer_iterator<U, Tag>& right) noexcept
		{
			return left.get() == right.get();
		}

		template <typename T, typename U, typename Tag,
			typename std::enable_if<
				std::is_same<
					typename std::remove_const<T>::type,
					typename std::remove_const<U>::type
				>::value,
			int>::type = 0>
		LINGO_CONSTEXPR11 bool operator != (const pointer_iterator<T, Tag>& left, const pointer_iterator<U, Tag>& right) noexcept
		{
			return left.get() != right.get();
		}
		
		template <typename T, typename U, typename Tag,
			typename std::enable_if<
				std::is_same<
					typename std::remove_const<T>::type,
					typename std::remove_const<U>::type
				>::value,
			int>::type = 0>
		LINGO_CONSTEXPR11 bool operator < (const pointer_iterator<T, Tag>& left, const pointer_iterator<U, Tag>& right) noexcept
		{
			return left.get() < right.get();
		}

		template <typename T, typename U, typename Tag,
			typename std::enable_if<
				std::is_same<
					typename std::remove_const<T>::type,
					typename std::remove_const<U>::type
				>::value,
			int>::type = 0>
		LINGO_CONSTEXPR11 bool operator > (const pointer_iterator<T, Tag>& left, const pointer_iterator<U, Tag>& right) noexcept
		{
			return left.get() > right.get();
		}
		
		template <typename T, typename U, typename Tag,
			typename std::enable_if<
				std::is_same<
					typename std::remove_const<T>::type,
					typename std::remove_const<U>::type
				>::value,
			int>::type = 0>
		LINGO_CONSTEXPR11 bool operator <= (const pointer_iterator<T, Tag>& left, const pointer_iterator<U, Tag>& right) noexcept
		{
			return left.get() <= right.get();
		}

		template <typename T, typename U, typename Tag,
			typename std::enable_if<
				std::is_same<
					typename std::remove_const<T>::type,
					typename std::remove_const<U>::type
				>::value,
			int>::type = 0>
		LINGO_CONSTEXPR11 bool operator >= (const pointer_iterator<T, Tag>& left, const pointer_iterator<U, Tag>& right) noexcept
		{
			return left.get() >= right.get();
		}
	}
}

#endif