#ifndef H_LINGO_UTILITY_SPAN
#define H_LINGO_UTILITY_SPAN

#include <lingo/platform/constexpr.hpp>

#include <lingo/utility/pointer_iterator.hpp>

#include <cassert>
#include <cstddef>
#include <iterator>
#include <type_traits>

namespace lingo
{
	namespace utility
	{
		template <typename T>
		class span
		{
			public:
			using value_type = T;
			using reference = value_type&;
			using const_reference = const value_type&;
			using pointer = value_type*;
			using const_pointer = const value_type*;

			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;

			struct iterator_tag;
			using const_iterator = utility::pointer_iterator<const value_type, iterator_tag>;
			using iterator = const_iterator;
			using const_reverse_iterator = std::reverse_iterator<const_iterator>;
			using reverse_iterator = const_reverse_iterator;

			static LINGO_CONSTEXPR11 const size_type npos = static_cast<size_type>(-1);

			LINGO_CONSTEXPR11 span() noexcept:
				_data(nullptr),
				_size(0)
			{
			}

			LINGO_CONSTEXPR11 span(value_type* data, size_type size) noexcept:
				_data(data),
				_size(size)
			{
			}

			LINGO_CONSTEXPR11 span(value_type* first, value_type* last) noexcept:
				_data(first),
				_size(last - first)
			{
			}

			template <size_type N>
			LINGO_CONSTEXPR11 span(value_type(&data)[N]) noexcept:
				_data(data),
				_size(N)
			{
			}

			template <typename U, typename std::enable_if<std::is_convertible<typename span<U>::reference, reference>::value, int>::type = 0>
			LINGO_CONSTEXPR11 span(const span<U>& s) noexcept:
				_data(static_cast<pointer>(s.data())),
				_size(s.size())
			{
			}

			LINGO_CONSTEXPR11 pointer data() const noexcept
			{
				return _data;
			}

			LINGO_CONSTEXPR11 size_type size() const noexcept
			{
				return _size;
			}

			LINGO_CONSTEXPR11 reference front() const noexcept
			{
				return _data[0];
			}

			LINGO_CONSTEXPR11 reference back() const noexcept
			{
				return _data[_size - 1];
			}

			LINGO_CONSTEXPR11 span subspan(size_type offset) const noexcept
			{
				return subspan(offset, npos);
			}

			LINGO_CONSTEXPR11 span subspan(size_type offset, size_type count) const noexcept
			{
				return span(_data + offset, _size - offset < count ? _size - offset : count);
			}

			LINGO_CONSTEXPR11 span diff(const span& other) const noexcept
			{
				return span(_data, other._data - _data);
			}

			LINGO_CONSTEXPR11 reference operator [] (size_type index) const noexcept
			{
				assert(index < _size);
				return _data[index];
			}

			private:
			value_type* _data;
			size_type _size;
		};
	}
}

#endif