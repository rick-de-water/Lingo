#ifndef H_LINGO_STRING
#define H_LINGO_STRING

#include <lingo/platform/constexpr.hpp>
#include <lingo/string_converter.hpp>
#include <lingo/string_storage.hpp>
#include <lingo/string_view.hpp>

#include <lingo/encoding/cstring.hpp>
#include <lingo/encoding/endian.hpp>
#include <lingo/encoding/none.hpp>
#include <lingo/encoding/point_iterator.hpp>
#include <lingo/encoding/utf8.hpp>
#include <lingo/encoding/utf16.hpp>
#include <lingo/encoding/utf32.hpp>

#include <lingo/page/ascii.hpp>
#include <lingo/page/cstring.hpp>
#include <lingo/page/point_mapper.hpp>
#include <lingo/page/unicode.hpp>

#include <lingo/platform/endian.hpp>

#include <cassert>
#include <iterator>
#include <memory>
#include <type_traits>
#include <string>

namespace lingo
{
	namespace internal
	{
		template <typename Encoding>
		using default_allocator = std::allocator<typename Encoding::unit_type>;
	}

	template <typename Encoding, typename Page, typename Allocator = internal::default_allocator<Encoding>>
	class basic_string
	{
		public:
		using encoding_type = Encoding;
		using page_type = Page;
		using allocator_type = Allocator;

		using unit_type = typename encoding_type::unit_type;
		using point_type = typename encoding_type::point_type;

		using value_type = unit_type;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = typename std::allocator_traits<allocator_type>::pointer;
		using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;

		using size_type = typename std::allocator_traits<allocator_type>::size_type;
		using difference_type = typename std::allocator_traits<allocator_type>::difference_type;

		using iterator = pointer;
		using const_iterator = const_pointer;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		static LINGO_CONSTEXPR11 size_type npos = static_cast<size_type>(-1);
		static LINGO_CONSTEXPR11 unit_type null_terminator = unit_type{};

		private:
		static_assert(std::is_same<typename page_type::point_type, typename encoding_type::point_type>::value, "page_type::point_type must be the same type as encoding_type::point_type");
		static_assert(std::is_same<typename allocator_type::value_type, typename encoding_type::unit_type>::value, "allocator_type::value_type must be the same type as encoding_type::unit_type");

		using storage_type = basic_string_storage<value_type, allocator_type>;
		using string_view = basic_string_view<encoding_type, page_type>;

		public:
		basic_string() noexcept(noexcept(allocator_type())):
			basic_string(allocator_type())
		{
		}

		explicit basic_string(const allocator_type& allocator) noexcept:
			_storage(allocator)
		{
		}

		basic_string(size_type count, point_type point, const allocator_type& allocator = allocator_type()):
			basic_string(allocator)
		{
			// Encode the point into units
			unit_type encoded_point[encoding_type::max_units];
			const auto result = encoding_type::encode_point(point, encoded_point, encoding_type::max_units);
			if (result.error != error::error_code::success)
			{
				throw error::exception(result.error);
			}

			// Allocate memory
			const size_type new_size = result.size * count;
			_storage.grow(new_size);

			// Fill memory
			for (size_type i = 0; i < count; ++i)
			{
				_storage.copy_construct(_storage.data() + i * result.size, encoded_point, result.size);
			}

			// Construct null terminator
			_storage.copy_construct(_storage.data() + new_size, &null_terminator, 1);

			// Set size
			_storage.set_size(new_size);
		}

		template <std::size_t N, typename Foo = int, typename std::enable_if<(
			std::is_same<Encoding, encoding::cstring_default_encoding_t<value_type>>::value &&
			std::is_same<Page, page::cstring_default_page_t<value_type>>::value), Foo>::type = N>
		basic_string(const value_type (&cstring)[N], const allocator_type& allocator = allocator_type()):
			basic_string(string_view(cstring, N - 1, cstring[N - 1] == value_type{}), allocator)
		{
		}

		basic_string(string_view string_view, const allocator_type& allocator = allocator_type()):
			basic_string(allocator)
		{
			const size_type required_size = string_view.size();

			_storage.grow(required_size);
			_storage.resize_copy_construct(required_size, string_view.data());
		}

		template <typename SourceEncoding, typename SourcePage>
		explicit basic_string(basic_string_view<SourceEncoding, SourcePage> string_view, const allocator_type& allocator = allocator_type()):
			basic_string(string_converter<SourceEncoding, SourcePage, encoding_type, page_type>().template convert<allocator_type>(string_view, allocator))
		{
		}

		template <typename SourceEncoding, typename SourcePage, typename SourceAllocator>
		explicit basic_string(basic_string<SourceEncoding, SourcePage, SourceAllocator> string, const allocator_type& allocator = allocator_type()):
			basic_string(string_converter<SourceEncoding, SourcePage, encoding_type, page_type>().template convert<allocator_type>(string, allocator))
		{
		}

		basic_string(const basic_string& string):
			basic_string(string, allocator_type())
		{
		}

		basic_string(const basic_string& string, const allocator_type& allocator):
			_storage(string._storage, allocator)
		{
		}

		basic_string(const basic_string& string, size_type pos):
			basic_string(string, pos, allocator_type())
		{
		}

		basic_string(const basic_string& string, size_type pos, const allocator_type& allocator):
			basic_string(string, pos, npos, allocator)
		{
		}

		basic_string(const basic_string& string, size_type pos, size_type count):
			basic_string(string, pos, count, allocator_type())
		{
		}

		basic_string(const basic_string& string, size_type pos, size_type count, const allocator_type& allocator):
			basic_string(string_view(string.data() + pos, count == npos ? string.size() - pos : count), allocator)
		{
		}

		basic_string(basic_string&& string):
			basic_string(std::move(string), allocator_type())
		{
		}

		basic_string(basic_string&& string, const allocator_type& allocator):
			_storage(std::move(string._storage), allocator)
		{
		}

		allocator_type get_allocator() const noexcept(std::is_nothrow_copy_constructible<allocator_type>::value)
		{
			return _storage.get_allocator();
		}

		iterator begin() noexcept
		{
			return iterator(data());
		}

		iterator end() noexcept
		{
			return iterator(data() + size());
		}

		const_iterator cbegin() const noexcept
		{
			return const_iterator(data());
		}

		const_iterator cend() const noexcept
		{
			return const_iterator(data() + size());
		}

		reverse_iterator rbegin() noexcept
		{
			return reverse_iterator(begin());
		}

		reverse_iterator rend() noexcept
		{
			return reverse_iterator(end());
		}

		const_reverse_iterator crbegin() const noexcept
		{
			return const_reverse_iterator(cend());
		}

		const_reverse_iterator crend() const noexcept
		{
			return const_reverse_iterator(cbegin());
		}

		reference operator [] (size_type pos) noexcept
		{
			return const_cast<reference>(static_cast<const basic_string*>(this)->operator[](pos));
		}

		const_reference operator [] (size_type pos) const noexcept
		{
			return data()[pos];
		}

		reference at(size_type pos)
		{
			return const_cast<reference>(static_cast<const basic_string*>(this)->at(pos));
		}

		const_reference at(size_type pos) const
		{
			if (pos < size())
			{
				return operator[](pos);
			}
			else
			{
				throw std::out_of_range("Index out of range");
			}
		}

		reference front() noexcept
		{
			return const_cast<reference>(static_cast<const basic_string*>(this)->front());
		}

		const_reference front() const noexcept
		{
			return operator[](0);
		}

		reference back() noexcept
		{
			return const_cast<reference>(static_cast<const basic_string*>(this)->back());
		}

		const_reference back() const noexcept
		{
			return operator[](size() - 1);
		}

		pointer data() noexcept
		{
			return const_cast<pointer>(static_cast<const basic_string*>(this)->data());
		}

		const_pointer data() const noexcept
		{
			return _storage.data();
		}

		size_type size() const noexcept
		{
			return _storage.size();
		}

		size_type length() const noexcept
		{
			return size();
		}

		size_type max_size() const noexcept
		{
			return _storage.max_size();
		}

		size_type capacity() const noexcept
		{
			return _storage.capacity();
		}

		bool empty() const noexcept
		{
			return size() == 0;
		}

		void resize(size_type size)
		{
			if (size == _storage.size())
			{
				return;
			}

			if (size > _storage.capacity())
			{
				_storage.grow(size);
			}

			_storage.resize_default_construct(size);
		}

		void reserve(size_type size)
		{
			_storage.grow(size);
		}

		void clear() noexcept
		{
			resize(0);
		}

		void push_back(point_type point)
		{
			// Encode the point into units
			unit_type encoded_point[encoding_type::max_units];
			const auto result = encoding_type::encode_point(point, encoded_point, encoding_type::max_units);
			if (result.error != error::error_code::success)
			{
				throw error::exception(result.error);
			}

			// Allocate memory
			_storage.grow(size() + result.size);

			// Append data
			append(string_view(encoded_point, result.size));
		}

		void insert(size_type index, string_view string, size_type count) noexcept(noexcept(std::declval<storage_type&>().grow(std::declval<size_t>())))
		{
			// Make sure that the index is valid
			assert(index <= size());

			// Empty strings and counts of 0 can be ignored
			if (string.size() == 0 || count == 0)
			{
				return;
			}

			size_type old_size = size();
			size_type new_size = size() + string.size() * count;

			// Allocate enough room 
			_storage.grow(new_size);

			// Destruct old data
			for (size_type i = index; i <= old_size; ++i)
			{
				_storage.destruct(_storage.data() + new_size, 1);
			}

			// Copy string count times
			for (size_type i = 0; i < count; ++i)
			{
				_storage.copy_construct(_storage.data() + index + string.size() * i, string.data(), string.size());
			}

			// Construct new null terminator
			_storage.copy_construct(_storage.data() + new_size, &null_terminator, 1);

			// Update size
			_storage.set_size(new_size);
		}

		template <typename OtherAllocator>
		basic_string& operator += (const basic_string<Encoding, Page, OtherAllocator>& other)
		{
			// We can't simply call the string_view version here
			// If &other == this, the data pointer in the string view might become invalid when growing the capacity

			// Allocate memory
			_storage.grow(size() + other.size());

			// Append data
			append(other);

			// Return this
			return *this;
		}

		basic_string& operator += (string_view other)
		{
			// Allocate memory
			_storage.grow(size() + other.size());

			// Append data
			append(other);

			// Return this
			return *this;
		}

		basic_string& operator += (point_type other)
		{
			// Append point
			push_back(other);

			// Return this
			return *this;
		}

		basic_string& operator = (const basic_string& string)
		{
			_storage = string._storage;
			return *this;
		}

		basic_string& operator = (basic_string&& string)
		{
			_storage = std::move(string._storage);
			return *this;
		}

		operator string_view() const noexcept
		{
			return string_view(data(), size(), true);
		}

		const_pointer c_str() const noexcept
		{
			return data();
		}

		template <typename Traits = std::char_traits<value_type>, typename StdAllocator = allocator_type>
		std::basic_string<value_type, Traits, StdAllocator> std() const
		{
			return std::basic_string<value_type, Traits, StdAllocator>(data(), size());
		}

		template <typename RightAllocator>
		LINGO_CONSTEXPR14 int compare(const basic_string<Encoding, Page, RightAllocator>& other) const noexcept(noexcept(std::declval<const basic_string&>().compare(other.operator lingo::basic_string_view<Encoding, Page>())))
		{
			return compare(other.operator lingo::basic_string_view<Encoding, Page>());
		}

		LINGO_CONSTEXPR14 int compare(basic_string_view<Encoding, Page> other) const
		{
			return operator lingo::basic_string_view<Encoding, Page>().compare(other);
		}

		template <typename _ = int, typename std::enable_if<
			std::is_same<encoding::cstring_default_encoding_t<unit_type>, encoding_type>::value &&
			std::is_same<page::cstring_default_page_t<unit_type>, page_type>::value, _>::type = 0>
		LINGO_CONSTEXPR14 int compare(const unit_type* other) const noexcept(noexcept(std::declval<const basic_string&>().compare(string_view(other))))
		{
			return compare(string_view(other));
		}

		private:
		void append(string_view other)
		{
			const size_type current_size = size();
			const size_type added_size = other.size();
			const size_type new_size = current_size + added_size;

			assert(new_size <= capacity());

			// Destruct old null terminator
			_storage.destruct(_storage.data() + current_size, 1);

			// Fill memory
			_storage.copy_construct(_storage.data() + current_size, other.data(), added_size);

			// Construct new null terminator
			_storage.copy_construct(_storage.data() + new_size, &null_terminator, 1);

			// Update size
			_storage.set_size(new_size);
		}

		storage_type _storage;
	};

	template <typename Encoding, typename Page, typename Allocator>
	LINGO_CONSTEXPR11 typename basic_string<Encoding, Page, Allocator>::size_type basic_string<Encoding, Page, Allocator>::npos;
	template <typename Encoding, typename Page, typename Allocator>
	LINGO_CONSTEXPR11 typename basic_string<Encoding, Page, Allocator>::unit_type basic_string<Encoding, Page, Allocator>::null_terminator;

	template <typename Encoding, typename Page, typename LeftAllocator, typename RightAllocator, typename ResultAllocator = LeftAllocator>
	basic_string<Encoding, Page, ResultAllocator> operator + (basic_string<Encoding, Page, LeftAllocator> left, basic_string<Encoding, Page, RightAllocator> right)
	{
		return operator+<Encoding, Page, ResultAllocator>(
			left.operator lingo::basic_string_view<Encoding, Page>(),
			right.operator lingo::basic_string_view<Encoding, Page>());
	}

	template <typename Encoding, typename Page, typename LeftAllocator, typename ResultAllocator = LeftAllocator>
	basic_string<Encoding, Page, ResultAllocator> operator + (basic_string<Encoding, Page, LeftAllocator> left, basic_string_view<Encoding, Page> right)
	{
		return operator+<Encoding, Page, ResultAllocator>(
			left.operator lingo::basic_string_view<Encoding, Page>(),
			right);
	}

	template <typename Encoding, typename Page, typename RightAllocator, typename ResultAllocator = RightAllocator>
	basic_string<Encoding, Page, ResultAllocator> operator + (basic_string_view<Encoding, Page> left, basic_string<Encoding, Page, RightAllocator> right)
	{
		return operator+<Encoding, Page, ResultAllocator>(
			left,
			right.operator lingo::basic_string_view<Encoding, Page>());
	}

	template <typename Encoding, typename Page, typename ResultAllocator = internal::default_allocator<Encoding>>
	basic_string<Encoding, Page, ResultAllocator> operator + (basic_string_view<Encoding, Page> left, basic_string_view<Encoding, Page> right)
	{
		basic_string<Encoding, Page, ResultAllocator> result;
		result.reserve(left.size() + right.size());
		result += left;
		result += right;
		return result;
	}

	template <typename Encoding, typename Page, typename LeftAllocator, typename ResultAllocator = LeftAllocator>
	basic_string<Encoding, Page, ResultAllocator> operator + (basic_string<Encoding, Page, LeftAllocator> left, typename Encoding::point_type right)
	{
		basic_string<Encoding, Page, ResultAllocator> result;
		result.reserve(left.size() + Encoding::max_units);
		result = left;
		result.push_back(right);
		return result;
	}

	template <typename Encoding, typename Page, typename RightAllocator, typename ResultAllocator = RightAllocator>
	basic_string<Encoding, Page, ResultAllocator> operator + (typename Encoding::point_type left, basic_string<Encoding, Page, RightAllocator> right)
	{
		basic_string<Encoding, Page, ResultAllocator> result;
		result.reserve(right.size() + Encoding::max_units);
		result.push_back(left);
		result += right;
		return result;
	}

	template <typename Encoding, typename Page, typename LeftAllocator, typename RightAllocator>
	bool operator == (const basic_string<Encoding, Page, LeftAllocator>& left, const basic_string<Encoding, Page, RightAllocator>& right) noexcept(noexcept(left.compare(right)))
	{
		return left.compare(right) == 0;
	}

	template <typename Encoding, typename Page, typename LeftAllocator, typename RightAllocator>
	bool operator != (const basic_string<Encoding, Page, LeftAllocator>& left, const basic_string<Encoding, Page, RightAllocator>& right) noexcept(noexcept(left.compare(right)))
	{
		return left.compare(right) != 0;
	}

	template <typename Encoding, typename Page, typename LeftAllocator, typename RightAllocator>
	bool operator < (const basic_string<Encoding, Page, LeftAllocator>& left, const basic_string<Encoding, Page, RightAllocator>& right) noexcept(noexcept(left.compare(right)))
	{
		return left.compare(right) < 0;
	}

	template <typename Encoding, typename Page, typename LeftAllocator, typename RightAllocator>
	bool operator > (const basic_string<Encoding, Page, LeftAllocator>& left, const basic_string<Encoding, Page, RightAllocator>& right) noexcept(noexcept(left.compare(right)))
	{
		return left.compare(right) > 0;
	}

	template <typename Encoding, typename Page, typename LeftAllocator, typename RightAllocator>
	bool operator <= (const basic_string<Encoding, Page, LeftAllocator>& left, const basic_string<Encoding, Page, RightAllocator>& right) noexcept(noexcept(left.compare(right)))
	{
		return left.compare(right) <= 0;
	}

	template <typename Encoding, typename Page, typename LeftAllocator, typename RightAllocator>
	bool operator >= (const basic_string<Encoding, Page, LeftAllocator>& left, const basic_string<Encoding, Page, RightAllocator>& right) noexcept(noexcept(left.compare(right)))
	{
		return left.compare(right) >= 0;
	}


	template <typename Encoding, typename Page, typename LeftAllocator>
	bool operator == (const basic_string<Encoding, Page, LeftAllocator>& left, basic_string_view<Encoding, Page> right) noexcept(noexcept(left.compare(right)))
	{
		return left.compare(right) == 0;
	}

	template <typename Encoding, typename Page, typename LeftAllocator>
	bool operator != (const basic_string<Encoding, Page, LeftAllocator>& left, basic_string_view<Encoding, Page> right) noexcept(noexcept(left.compare(right)))
	{
		return left.compare(right) != 0;
	}

	template <typename Encoding, typename Page, typename LeftAllocator>
	bool operator < (const basic_string<Encoding, Page, LeftAllocator>& left, basic_string_view<Encoding, Page> right) noexcept(noexcept(left.compare(right)))
	{
		return left.compare(right) < 0;
	}

	template <typename Encoding, typename Page, typename LeftAllocator>
	bool operator > (const basic_string<Encoding, Page, LeftAllocator>& left, basic_string_view<Encoding, Page> right) noexcept(noexcept(left.compare(right)))
	{
		return left.compare(right) > 0;
	}

	template <typename Encoding, typename Page, typename LeftAllocator>
	bool operator <= (const basic_string<Encoding, Page, LeftAllocator>& left, basic_string_view<Encoding, Page> right) noexcept(noexcept(left.compare(right)))
	{
		return left.compare(right) <= 0;
	}

	template <typename Encoding, typename Page, typename LeftAllocator>
	bool operator >= (const basic_string<Encoding, Page, LeftAllocator>& left, basic_string_view<Encoding, Page> right) noexcept(noexcept(left.compare(right)))
	{
		return left.compare(right) >= 0;
	}


	template <typename Encoding, typename Page, typename RightAllocator>
	bool operator == (basic_string_view<Encoding, Page> left, const basic_string<Encoding, Page, RightAllocator>& right) noexcept(noexcept(left.compare(right)))
	{
		return left.compare(right) == 0;
	}

	template <typename Encoding, typename Page, typename RightAllocator>
	bool operator != (basic_string_view<Encoding, Page> left, const basic_string<Encoding, Page, RightAllocator>& right) noexcept(noexcept(left.compare(right)))
	{
		return left.compare(right) != 0;
	}

	template <typename Encoding, typename Page, typename RightAllocator>
	bool operator < (basic_string_view<Encoding, Page> left, const basic_string<Encoding, Page, RightAllocator>& right) noexcept(noexcept(left.compare(right)))
	{
		return left.compare(right) < 0;
	}

	template <typename Encoding, typename Page, typename RightAllocator>
	bool operator > (basic_string_view<Encoding, Page> left, const basic_string<Encoding, Page, RightAllocator>& right) noexcept(noexcept(left.compare(right)))
	{
		return left.compare(right) > 0;
	}

	template <typename Encoding, typename Page, typename RightAllocator>
	bool operator <= (basic_string_view<Encoding, Page> left, const basic_string<Encoding, Page, RightAllocator>& right) noexcept(noexcept(left.compare(right)))
	{
		return left.compare(right) <= 0;
	}

	template <typename Encoding, typename Page, typename RightAllocator>
	bool operator >= (basic_string_view<Encoding, Page> left, const basic_string<Encoding, Page, RightAllocator>& right) noexcept(noexcept(left.compare(right)))
	{
		return left.compare(right) >= 0;
	}


	template <typename Encoding, typename Page, typename LeftAllocator, typename std::enable_if<
		std::is_same<encoding::cstring_default_encoding_t<typename Encoding::unit_type>, Encoding>::value &&
		std::is_same<page::cstring_default_page_t<typename Encoding::unit_type>, Page>::value, int>::type = 0>
	bool operator == (const basic_string<Encoding, Page, LeftAllocator>& left, const typename Encoding::unit_type* right) noexcept(noexcept(left.compare(right)))
	{
		return left.compare(right) == 0;
	}

	template <typename Encoding, typename Page, typename LeftAllocator, typename std::enable_if<
		std::is_same<encoding::cstring_default_encoding_t<typename Encoding::unit_type>, Encoding>::value &&
		std::is_same<page::cstring_default_page_t<typename Encoding::unit_type>, Page>::value, int>::type = 0>
	bool operator != (const basic_string<Encoding, Page, LeftAllocator>& left, const typename Encoding::unit_type* right) noexcept(noexcept(left.compare(right)))
	{
		return left.compare(right) != 0;
	}

	template <typename Encoding, typename Page, typename LeftAllocator, typename std::enable_if<
		std::is_same<encoding::cstring_default_encoding_t<typename Encoding::unit_type>, Encoding>::value &&
		std::is_same<page::cstring_default_page_t<typename Encoding::unit_type>, Page>::value, int>::type = 0>
	bool operator < (const basic_string<Encoding, Page, LeftAllocator>& left, const typename Encoding::unit_type* right) noexcept(noexcept(left.compare(right)))
	{
		return left.compare(right) < 0;
	}

	template <typename Encoding, typename Page, typename LeftAllocator, typename std::enable_if<
		std::is_same<encoding::cstring_default_encoding_t<typename Encoding::unit_type>, Encoding>::value &&
		std::is_same<page::cstring_default_page_t<typename Encoding::unit_type>, Page>::value, int>::type = 0>
	bool operator > (const basic_string<Encoding, Page, LeftAllocator>& left, const typename Encoding::unit_type* right) noexcept(noexcept(left.compare(right)))
	{
		return left.compare(right) > 0;
	}

	template <typename Encoding, typename Page, typename LeftAllocator, typename std::enable_if<
		std::is_same<encoding::cstring_default_encoding_t<typename Encoding::unit_type>, Encoding>::value &&
		std::is_same<page::cstring_default_page_t<typename Encoding::unit_type>, Page>::value, int>::type = 0>
	bool operator <= (const basic_string<Encoding, Page, LeftAllocator>& left, const typename Encoding::unit_type* right) noexcept(noexcept(left.compare(right)))
	{
		return left.compare(right) <= 0;
	}

	template <typename Encoding, typename Page, typename LeftAllocator, typename std::enable_if<
		std::is_same<encoding::cstring_default_encoding_t<typename Encoding::unit_type>, Encoding>::value &&
		std::is_same<page::cstring_default_page_t<typename Encoding::unit_type>, Page>::value, int>::type = 0>
	bool operator >= (const basic_string<Encoding, Page, LeftAllocator>& left, const typename Encoding::unit_type* right) noexcept(noexcept(left.compare(right)))
	{
		return left.compare(right) >= 0;
	}


	template <typename Encoding, typename Page, typename RightAllocator, typename std::enable_if<
		std::is_same<encoding::cstring_default_encoding_t<typename Encoding::unit_type>, Encoding>::value &&
		std::is_same<page::cstring_default_page_t<typename Encoding::unit_type>, Page>::value, int>::type = 0>
	bool operator == (const typename Encoding::unit_type* left, const basic_string<Encoding, Page, RightAllocator>& right) noexcept(noexcept(right.compare(left)))
	{
		return right.compare(left) == 0;
	}

	template <typename Encoding, typename Page, typename RightAllocator, typename std::enable_if<
		std::is_same<encoding::cstring_default_encoding_t<typename Encoding::unit_type>, Encoding>::value &&
		std::is_same<page::cstring_default_page_t<typename Encoding::unit_type>, Page>::value, int>::type = 0>
	bool operator != (const typename Encoding::unit_type* left, const basic_string<Encoding, Page, RightAllocator>& right) noexcept(noexcept(right.compare(left)))
	{
		return right.compare(left) != 0;
	}

	template <typename Encoding, typename Page, typename RightAllocator, typename std::enable_if<
		std::is_same<encoding::cstring_default_encoding_t<typename Encoding::unit_type>, Encoding>::value &&
		std::is_same<page::cstring_default_page_t<typename Encoding::unit_type>, Page>::value, int>::type = 0>
	bool operator < (const typename Encoding::unit_type* left, const basic_string<Encoding, Page, RightAllocator>& right) noexcept(noexcept(right.compare(left)))
	{
		return right.compare(left) > 0;
	}

	template <typename Encoding, typename Page, typename RightAllocator, typename std::enable_if<
		std::is_same<encoding::cstring_default_encoding_t<typename Encoding::unit_type>, Encoding>::value &&
		std::is_same<page::cstring_default_page_t<typename Encoding::unit_type>, Page>::value, int>::type = 0>
	bool operator > (const typename Encoding::unit_type* left, const basic_string<Encoding, Page, RightAllocator>& right) noexcept(noexcept(right.compare(left)))
	{
		return right.compare(left) < 0;
	}

	template <typename Encoding, typename Page, typename RightAllocator, typename std::enable_if<
		std::is_same<encoding::cstring_default_encoding_t<typename Encoding::unit_type>, Encoding>::value &&
		std::is_same<page::cstring_default_page_t<typename Encoding::unit_type>, Page>::value, int>::type = 0>
	bool operator <= (const typename Encoding::unit_type* left, const basic_string<Encoding, Page, RightAllocator>& right) noexcept(noexcept(right.compare(left)))
	{
		return right.compare(left) >= 0;
	}

	template <typename Encoding, typename Page, typename RightAllocator, typename std::enable_if<
		std::is_same<encoding::cstring_default_encoding_t<typename Encoding::unit_type>, Encoding>::value &&
		std::is_same<page::cstring_default_page_t<typename Encoding::unit_type>, Page>::value, int>::type = 0>
	bool operator >= (const typename Encoding::unit_type* left, const basic_string<Encoding, Page, RightAllocator>& right) noexcept(noexcept(right.compare(left)))
	{
		return right.compare(left) <= 0;
	}

	// Fixed page typedefs
	template <typename Encoding, typename Allocator = internal::default_allocator<Encoding>>
	using basic_ascii_string = basic_string<Encoding, page::ascii, Allocator>;
	template <typename Encoding, typename Allocator = internal::default_allocator<Encoding>>
	using basic_unicode_string = basic_string<Encoding, page::unicode, Allocator>;

	// Fixed encoding typedefs
	template <typename Unit, typename Allocator = internal::default_allocator<encoding::utf8<Unit, char32_t>>>
	using basic_utf8_string = basic_unicode_string<encoding::utf8<Unit, char32_t>, Allocator>;

	template <typename Unit, typename Allocator = internal::default_allocator<encoding::utf8<Unit, char32_t>>>
	using basic_utf16_string = basic_unicode_string<encoding::utf16<Unit, char32_t>, Allocator>;
	template <typename Unit, typename Allocator = internal::default_allocator<encoding::utf8<Unit, char32_t>>>
	using basic_utf16_le_string = basic_unicode_string<encoding::utf16_le<Unit, char32_t>, Allocator>;
	template <typename Unit, typename Allocator = internal::default_allocator<encoding::utf8<Unit, char32_t>>>
	using basic_utf16_be_string = basic_unicode_string<encoding::utf16_be<Unit, char32_t>, Allocator>;

	template <typename Unit, typename Allocator = internal::default_allocator<encoding::none<Unit, char32_t>>>
	using basic_utf32_string = basic_unicode_string<encoding::utf32<Unit, char32_t>, Allocator>;
	template <typename Unit, typename Allocator = internal::default_allocator<encoding::none<Unit, char32_t>>>
	using basic_utf32_le_string = basic_unicode_string<encoding::utf32_le<Unit, char32_t>, Allocator>;
	template <typename Unit, typename Allocator = internal::default_allocator<encoding::none<Unit, char32_t>>>
	using basic_utf32_be_string = basic_unicode_string<encoding::utf32_be<Unit, char32_t>, Allocator>;

	// Fixed encoding & page typedefs
	template <typename Allocator = internal::default_allocator<encoding::cstring_default_encoding_t<char>>>
	using basic_narrow_string = basic_string<encoding::cstring_default_encoding_t<char>, page::cstring_default_page_t<char>, Allocator>;
	template <typename Allocator = internal::default_allocator<encoding::cstring_default_encoding_t<wchar_t>>>
	using basic_wide_string = basic_string<encoding::cstring_default_encoding_t<wchar_t>, page::cstring_default_page_t<wchar_t>, Allocator>;

	// Fully specialized typedefs
	using narrow_string = basic_narrow_string<>;
	using wide_string = basic_wide_string<>;

	using ascii_string = basic_ascii_string<encoding::none<char, char>>;

	#ifdef __cpp_char8_t
	using utf8_string = basic_utf8_string<char8_t>;
	#else
	using utf8_string = basic_utf8_string<char>;
	#endif

	using utf16_string = basic_utf16_string<char16_t>;
	using utf16_le_string = basic_utf16_le_string<char16_t>;
	using utf16_be_string = basic_utf16_be_string<char16_t>;

	using utf32_string = basic_utf32_string<char32_t>;
	using utf32_le_string = basic_utf32_le_string<char32_t>;
	using utf32_be_string = basic_utf32_be_string<char32_t>;

	// Default string typedef
	using string = utf8_string;
}

#endif