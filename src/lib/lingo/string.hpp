#ifndef H_LINGO_STRING
#define H_LINGO_STRING

#include <lingo/platform/constexpr.hpp>
#include <lingo/string_converter.hpp>
#include <lingo/string_storage.hpp>
#include <lingo/string_view.hpp>

#include <lingo/encoding/execution.hpp>
#include <lingo/encoding/endian.hpp>
#include <lingo/encoding/none.hpp>
#include <lingo/encoding/point_iterator.hpp>
#include <lingo/encoding/utf8.hpp>
#include <lingo/encoding/utf16.hpp>
#include <lingo/encoding/utf32.hpp>

#include <lingo/page/ascii.hpp>
#include <lingo/page/execution.hpp>
#include <lingo/page/point_mapper.hpp>
#include <lingo/page/unicode.hpp>

#include <lingo/platform/endian.hpp>

#include <lingo/utility/object_builder.hpp>
#include <lingo/utility/pointer_iterator.hpp>
#include <lingo/utility/type_traits.hpp>

#include <cassert>
#include <iosfwd>
#include <iterator>
#include <memory>
#include <type_traits>
#include <string>
#include <stdexcept>

#ifdef __cpp_lib_string_view
#include <string_view>
#endif

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

		struct iterator_tag;
		using iterator = utility::pointer_iterator<value_type, iterator_tag>;
		using const_iterator = utility::pointer_iterator<const value_type, iterator_tag>;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		static LINGO_CONSTEXPR11 size_type npos = static_cast<size_type>(-1);
		static LINGO_CONSTEXPR11 unit_type null_terminator = unit_type{};
		static LINGO_CONSTEXPR11 bool is_execution_set = lingo::utility::is_execution_set<encoding_type, page_type>::value;

		private:
		static_assert(std::is_same<typename page_type::point_type, typename encoding_type::point_type>::value, "page_type::point_type must be the same type as encoding_type::point_type");
		static_assert(std::is_same<typename allocator_type::value_type, typename encoding_type::unit_type>::value, "allocator_type::value_type must be the same type as encoding_type::unit_type");

		using object_builder = utility::object_builder<value_type>;
		using storage_type = basic_string_storage<value_type, allocator_type>;
		using basic_string_view = lingo::basic_string_view<encoding_type, page_type>;

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
			assign(count, point);
		}

		template <typename _ = int, typename std::enable_if<is_execution_set, _>::type = 0>
		basic_string(const_pointer cstring, const allocator_type& allocator = allocator_type()):
			basic_string(basic_string_view(cstring), allocator)
		{
		}

		template <typename _ = int, typename std::enable_if<is_execution_set, _>::type = 0>
		basic_string(const_pointer cstring, size_type count, const allocator_type& allocator = allocator_type()):
			basic_string(basic_string_view(cstring, count), allocator)
		{
		}

		template <typename _ = int, typename std::enable_if<!is_execution_set, _>::type = 0>
		explicit basic_string(const_pointer cstring, const allocator_type& allocator = allocator_type()):
			basic_string(basic_string_view(cstring), allocator)
		{
		}

		template <typename _ = int, typename std::enable_if<!is_execution_set, _>::type = 0>
		explicit basic_string(const_pointer cstring, size_type count, const allocator_type& allocator = allocator_type()):
			basic_string(basic_string_view(cstring, count), allocator)
		{
		}

		basic_string(basic_string_view basic_string_view, const allocator_type& allocator = allocator_type()):
			basic_string(allocator)
		{
			assign(basic_string_view);
		}

		template <typename SourceEncoding, typename SourcePage>
		explicit basic_string(lingo::basic_string_view<SourceEncoding, SourcePage> basic_string_view, const allocator_type& allocator = allocator_type()):
			basic_string(string_converter<SourceEncoding, SourcePage, encoding_type, page_type>().template convert<allocator_type>(basic_string_view, allocator))
		{
		}

		template <typename SourceEncoding, typename SourcePage, typename SourceAllocator>
		explicit basic_string(basic_string<SourceEncoding, SourcePage, SourceAllocator> string, const allocator_type& allocator = allocator_type()):
			basic_string(string_converter<SourceEncoding, SourcePage, encoding_type, page_type>().template convert<allocator_type>(string, allocator))
		{
		}

		basic_string(const basic_string& str):
			basic_string(str, allocator_type())
		{
		}

		basic_string(const basic_string& str, const allocator_type& allocator):
			_storage(str._storage, allocator)
		{
		}

		basic_string(const basic_string& str, size_type pos):
			basic_string(str, pos, allocator_type())
		{
		}

		basic_string(const basic_string& str, size_type pos, const allocator_type& allocator):
			basic_string(str, pos, npos, allocator)
		{
		}

		basic_string(const basic_string& str, size_type pos, size_type count):
			basic_string(str, pos, count, allocator_type())
		{
		}

		basic_string(const basic_string& str, size_type pos, size_type count, const allocator_type& allocator):
			basic_string(basic_string_view(str.data() + pos, count == npos ? str.size() - pos : count), allocator)
		{
		}

		basic_string(basic_string&& str):
			basic_string(std::move(str), allocator_type())
		{
		}

		basic_string(basic_string&& str, const allocator_type& allocator):
			_storage(std::move(str._storage), allocator)
		{
		}

		template <typename SourceAllocator, typename _ = int, typename std::enable_if<is_execution_set, _>::type = 0>
		basic_string(const std::basic_string<value_type, std::char_traits<value_type>, SourceAllocator>& str, const allocator_type& allocator = allocator_type()):
			basic_string(basic_string_view(str.data(), str.size(), true), allocator)
		{
		}

		#ifdef __cpp_lib_string_view
		template <typename _ = int, typename std::enable_if<is_execution_set, _>::type = 0>
		basic_string(const std::basic_string_view<value_type, std::char_traits<value_type>>& str, const allocator_type& allocator = allocator_type()):
			basic_string(basic_string_view(str.data(), str.size(), false), allocator)
		{
		}
		#endif

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
			return reverse_iterator(end());
		}

		reverse_iterator rend() noexcept
		{
			return reverse_iterator(begin());
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

		void shrink_to_fit()
		{
			// TODO: actually shrink
		}

		bool empty() const noexcept
		{
			return size() == 0;
		}

		void resize(size_type new_size)
		{
			const size_type original_size = size();
			if (new_size == original_size)
			{
				return;
			}

			_storage.grow_append(new_size);

			if (new_size > original_size)
			{
				for (size_type i = original_size; i < new_size; ++i)
				{
					object_builder::copy_construct(data() + i, &null_terminator, 1);
				}
			}
			else
			{
				object_builder::destruct(data() + new_size, original_size - new_size);
			}

			object_builder::copy_construct(data() + new_size, &null_terminator, 1);
			_storage.set_size(new_size);
		}

		void reserve(size_type reserved_size)
		{
			const size_type original_size = size();
			_storage.grow_reserve(reserved_size);
			_storage.set_size(original_size);
		}

		void clear() noexcept
		{
			resize(0);
		}

		void assign(size_type count, point_type point)
		{
			// Encode the point into units
			unit_type encoded_point[encoding_type::max_units];
			// TODO: multipoint
			const auto result = encoding_type::encode_one(utility::span<const point_type>(&point, 1), encoded_point);
			if (result.error != error::error_code::success)
			{
				throw error::exception(result.error);
			}

			// Allocate memory
			const size_t point_size = result.destination.data() - encoded_point;
			const size_type destination_size = point_size * count;
			_storage.grow_discard(destination_size);
			const pointer destination_data = data();

			// Fill memory
			for (size_type i = 0; i < count; ++i)
			{
				object_builder::copy_construct(destination_data + i * point_size, encoded_point, point_size);
			}

			// Construct null terminator
			object_builder::copy_construct(destination_data + destination_size, &null_terminator, 1);

			// Update size
			_storage.set_size(destination_size);
		}

		void assign(const basic_string& str)
		{
			if (this != &str)
			{
				assign(str.operator lingo::basic_string_view<Encoding, Page>());
			}
		}

		void assign(const basic_string& str, size_type pos)
		{
			if (pos > str.size())
			{
				throw std::out_of_range("pos is out of range");
			}

			if (this != &str)
			{
				assign(str.operator lingo::basic_string_view<Encoding, Page>());
			}
		}

		void assign(const basic_string& str, size_type pos, size_type count)
		{
			if (pos > str.size())
			{
				throw std::out_of_range("pos is out of range");
			}

			if (this != &str)
			{
				assign(basic_string_view(str.data() + pos, std::min(count, str.length() - pos)));
			}
		}

		void assign(basic_string&& str)
		{
			if (this != &str)
			{
				_storage = std::move(str._storage);
			}
		}

		void assign(basic_string_view str)
		{
			const const_pointer source_data = str.data();
			const size_type source_size = str.size();

			_storage.grow_discard(source_size);
			_storage.assign(source_data, source_size);
		}

		void assign(basic_string_view str, size_type pos)
		{
			assign(basic_string_view(str.data() + pos, str.length() - pos));
		}

		void assign(basic_string_view str, size_type pos, size_type count)
		{
			assign(basic_string_view(str.data() + pos, std::min(count, str.length() - pos)));
		}

		template <typename _ = int, typename std::enable_if<is_execution_set, _>::type = 0>
		void assign(const_pointer str)
		{
			assign(basic_string_view(str));
		}

		template <typename _ = int, typename std::enable_if<is_execution_set, _>::type = 0>
		void assign(const_pointer str, size_type count)
		{
			assign(basic_string_view(str, count));
		}

		template <typename T,
			typename std::enable_if<
				!std::is_same<T, basic_string_view>::value &&
				!std::is_convertible<const T&, const_pointer>::value &&
				std::is_convertible<const T&, basic_string_view>::value
			>::type = 0>
		void assign(const T& str)
		{
			assign(basic_string_view(str));
		}

		template <typename T,
			typename std::enable_if<
				!std::is_same<T, basic_string_view>::value &&
				!std::is_convertible<const T&, const_pointer>::value &&
				std::is_convertible<const T&, basic_string_view>::value
			>::type = 0>
		void assign(const T& str, size_type pos)
		{
			assign(basic_string_view(str), pos);
		}

		template <typename T,
			typename std::enable_if<
				!std::is_same<T, basic_string_view>::value &&
				!std::is_convertible<const T&, const_pointer>::value &&
				std::is_convertible<const T&, basic_string_view>::value
			>::type = 0>
		void assign(const T& str, size_type pos, size_type count)
		{
			assign(basic_string_view(str), pos, count);
		}

		void append(size_type count, point_type point)
		{
			// Encode the point into units
			unit_type encoded_point[encoding_type::max_units];
			// TODO: multipoint
			const auto result = encoding_type::encode_one(utility::span<point_type>(&point, 1), encoded_point);
			if (result.error != error::error_code::success)
			{
				throw error::exception(result.error);
			}

			// Allocate memory
			const size_t point_size = result.destination.data() - encoded_point;
			const size_type original_size = size();
			const size_type destination_size = point_size * count + original_size;
			_storage.grow_append(destination_size);
			const pointer destination_data = data();

			// Fill memory
			for (size_type i = 0; i < count; ++i)
			{
				object_builder::copy_construct(destination_data + original_size + i * point_size, encoded_point, point_size);
			}

			// Construct null terminator
			object_builder::copy_construct(destination_data + destination_size, &null_terminator, 1);

			// Update size
			_storage.set_size(destination_size);
		}

		void append(const basic_string& str)
		{
			append(str, 0, npos);
		}

		void append(const basic_string& str, size_type pos)
		{
			append(str, pos, npos);
		}

		void append(const basic_string& str, size_type pos, size_type count)
		{
			const size_type original_size = size();
			const size_type source_size = std::min(str.size() - pos, count);
			const size_type new_size = original_size + source_size;
			_storage.grow_append(new_size);

			const const_pointer source_data = str.data() + pos;
			_storage.append(original_size, source_data, source_size);
		}

		void append(basic_string_view str)
		{
			append(str, 0, npos);
		}

		void append(basic_string_view str, size_type pos)
		{
			append(str, pos, npos);
		}

		void append(basic_string_view str, size_type pos, size_type count)
		{
			// Assert for self append
			assert(str.data() < data() || str.data() > data() + capacity());

			const size_type original_size = size();
			const size_type source_size = std::min(str.size() - pos, count);
			const size_type new_size = original_size + source_size;
			_storage.grow_append(new_size);

			const const_pointer source_data = str.data() + pos;
			_storage.append(original_size, source_data, source_size);
		}

		template <typename _ = int, typename std::enable_if<is_execution_set, _>::type = 0>
		void append(const_pointer str)
		{
			append(basic_string_view(str));
		}

		template <typename _ = int, typename std::enable_if<is_execution_set, _>::type = 0>
		void append(const_pointer str, size_type count)
		{
			append(basic_string_view(str, count));
		}

		template <typename T,
			typename std::enable_if<
				!std::is_same<T, basic_string_view>::value &&
				!std::is_convertible<const T&, const_pointer>::value &&
				std::is_convertible<const T&, basic_string_view>::value
			>::type = 0>
		void append(const T& str)
		{
			append(basic_string_view(str));
		}

		template <typename T,
			typename std::enable_if<
				!std::is_same<T, basic_string_view>::value &&
				!std::is_convertible<const T&, const_pointer>::value &&
				std::is_convertible<const T&, basic_string_view>::value
			>::type = 0>
		void append(const T& str, size_type pos)
		{
			append(basic_string_view(str), pos);
		}

		template <typename T,
			typename std::enable_if<
				!std::is_same<T, basic_string_view>::value &&
				!std::is_convertible<const T&, const_pointer>::value &&
				std::is_convertible<const T&, basic_string_view>::value
			>::type = 0>
		void append(const T& str, size_type pos, size_type count)
		{
			append(basic_string_view(str), pos, count);
		}

		void insert(size_type index, basic_string_view string, size_type count) noexcept(noexcept(std::declval<storage_type&>().grow(std::declval<size_t>())))
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
			_storage.grow_append(new_size); // TODO: grow_move

			// Destruct old data (null terminator was destructed by grow_append)
			for (size_type i = index; i < old_size; ++i)
			{
				object_builder::destruct(_storage.data() + new_size, 1);
			}

			// Copy string count times
			for (size_type i = 0; i < count; ++i)
			{
				object_builder::copy_construct(_storage.data() + index + string.size() * i, string.data(), string.size());
			}

			// Construct new null terminator
			object_builder::copy_construct(_storage.data() + new_size, &null_terminator, 1);

			// Update size
			_storage.set_size(new_size);
		}

		basic_string substr(size_type pos = 0, size_type count = npos) const
		{
			return basic_string(*this, pos, count);
		}

		size_type copy(value_type* dest, size_type count, size_type pos = 0) const
		{
			return view().copy(dest, count, pos);
		}

		template <typename OtherAllocator>
		basic_string& operator += (const basic_string<Encoding, Page, OtherAllocator>& other)
		{
			// We can't simply call the basic_string_view version here
			// If &other == this, the data pointer in the string view might become invalid when growing the capacity
			append(other);
			return *this;
		}

		basic_string& operator += (basic_string_view other)
		{
			append(other);
			return *this;
		}

		basic_string& operator += (point_type other)
		{
			append(1, other);
			return *this;
		}
		
		template <typename _ = int,
			typename std::enable_if<is_execution_set, _>::type = 0>
		basic_string& operator += (const value_type* other)
		{
			return (*this) += basic_string_view(other);
		}

		basic_string& operator = (const basic_string& str)
		{
			if (this != &str)
			{
				assign(str);
			}

			return *this;
		}

		basic_string& operator = (basic_string&& str)
		{
			if (this != &str)
			{
				assign(std::move(str));
			}

			return *this;
		}

		operator basic_string_view() const noexcept
		{
			return view();
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

		basic_string_view view() const noexcept
		{
			return basic_string_view(data(), size(), true);
		}

		template <typename RightAllocator>
		LINGO_CONSTEXPR14 int compare(const basic_string<encoding_type, page_type, RightAllocator>& str) const noexcept(noexcept(std::declval<const basic_string&>().compare(str.view())))
		{
			return compare(str.view());
		}

		LINGO_CONSTEXPR14 int compare(basic_string_view str) const
		{
			return view().compare(str);
		}

		template <typename _ = int, typename std::enable_if<is_execution_set, _>::type = 0>
		LINGO_CONSTEXPR14 int compare(const_pointer str) const noexcept(noexcept(std::declval<const basic_string&>().compare(basic_string_view(str))))
		{
			return compare(basic_string_view(str));
		}

		LINGO_CONSTEXPR14 size_type find(const basic_string& str) const noexcept
		{
			return find(str.view());
		}

		LINGO_CONSTEXPR14 size_type find(const basic_string& str, size_type pos) const noexcept
		{
			return find(str.view(), pos);
		}

		template <typename _ = int, typename std::enable_if<is_execution_set, _>::type = 0>
		LINGO_CONSTEXPR14 size_type find(const_pointer str) const noexcept
		{
			return find(basic_string_view(str));
		}

		template <typename _ = int, typename std::enable_if<is_execution_set, _>::type = 0>
		LINGO_CONSTEXPR14 size_type find(const_pointer str, size_type pos) const noexcept
		{
			return find(basic_string_view(str), pos);
		}

		template <typename _ = int, typename std::enable_if<is_execution_set, _>::type = 0>
		LINGO_CONSTEXPR14 size_type find(const_pointer str, size_type pos, size_type count) const noexcept
		{
			return find(basic_string_view(str, count, false), pos);
		}

		LINGO_CONSTEXPR14 size_type find(basic_string_view str) const noexcept
		{
			return find(str);
		}

		LINGO_CONSTEXPR14 size_type find(basic_string_view str, size_type pos) const noexcept
		{
			return view().find(str, pos);
		}

		template <typename T,
			typename std::enable_if<
				!std::is_same<T, basic_string_view>::value &&
				!std::is_convertible<const T&, const_pointer>::value &&
				std::is_convertible<const T&, basic_string_view>::value
			>::type = 0>
		LINGO_CONSTEXPR14 size_type find(const T& str) const noexcept
		{
			return find(basic_string_view(str), 0);
		}

		template <typename T,
			typename std::enable_if<
				!std::is_same<T, basic_string_view>::value &&
				!std::is_convertible<const T&, const_pointer>::value &&
				std::is_convertible<const T&, basic_string_view>::value
			>::type = 0>
		LINGO_CONSTEXPR14 size_type find(const T& str, size_type pos) const noexcept
		{
			return find(basic_string_view(str), pos);
		}

		LINGO_CONSTEXPR14 size_type rfind(const basic_string& str) const noexcept
		{
			return rfind(str, npos);
		}

		LINGO_CONSTEXPR14 size_type rfind(const basic_string& str, size_type pos) const noexcept
		{
			return find(str.view(), pos);
		}

		template <typename _ = int, typename std::enable_if<is_execution_set, _>::type = 0>
		LINGO_CONSTEXPR14 size_type rfind(const_pointer str) const noexcept
		{
			return rfind(basic_string_view(str));
		}

		template <typename _ = int, typename std::enable_if<is_execution_set, _>::type = 0>
		LINGO_CONSTEXPR14 size_type rfind(const_pointer str, size_type pos) const noexcept
		{
			return rfind(basic_string_view(str), pos);
		}

		template <typename _ = int, typename std::enable_if<is_execution_set, _>::type = 0>
		LINGO_CONSTEXPR14 size_type rfind(const_pointer str, size_type pos, size_type count) const noexcept
		{
			return rfind(basic_string_view(str, count, false), pos);
		}

		LINGO_CONSTEXPR14 size_type rfind(basic_string_view str) const noexcept
		{
			return rfind(str);
		}

		LINGO_CONSTEXPR14 size_type rfind(basic_string_view str, size_type pos) const noexcept
		{
			return view().rfind(str, pos);
		}

		template <typename T,
			typename std::enable_if<
				!std::is_same<T, basic_string_view>::value &&
				!std::is_convertible<const T&, const_pointer>::value &&
				std::is_convertible<const T&, basic_string_view>::value
			>::type = 0>
		LINGO_CONSTEXPR14 size_type rfind(const T& str) const noexcept
		{
			return rfind(basic_string_view(str), npos);
		}

		template <typename T,
			typename std::enable_if<
				!std::is_same<T, basic_string_view>::value &&
				!std::is_convertible<const T&, const_pointer>::value &&
				std::is_convertible<const T&, basic_string_view>::value
			>::type = 0>
		LINGO_CONSTEXPR14 size_type rfind(const T& str, size_type pos) const noexcept
		{
			return rfind(basic_string_view(str), pos);
		}

		private:
		storage_type _storage;
	};

	template <typename Encoding, typename Page, typename Allocator>
	LINGO_CONSTEXPR11 typename basic_string<Encoding, Page, Allocator>::size_type basic_string<Encoding, Page, Allocator>::npos;
	template <typename Encoding, typename Page, typename Allocator>
	LINGO_CONSTEXPR11 typename basic_string<Encoding, Page, Allocator>::unit_type basic_string<Encoding, Page, Allocator>::null_terminator;
	template <typename Encoding, typename Page, typename Allocator>
	LINGO_CONSTEXPR11 bool basic_string<Encoding, Page, Allocator>::is_execution_set;

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
	basic_string<Encoding, Page, ResultAllocator> operator + (const basic_string_view<Encoding, Page>& left, const basic_string_view<Encoding, Page>& right)
	{
		basic_string<Encoding, Page, ResultAllocator> result;
		result.reserve(left.size() + right.size());
		result.append(left);
		result.append(right);
		return result;
	}

	template <typename Encoding, typename Page, typename LeftAllocator, typename ResultAllocator = LeftAllocator>
	basic_string<Encoding, Page, ResultAllocator> operator + (const basic_string<Encoding, Page, LeftAllocator>& left, typename Encoding::point_type right)
	{
		basic_string<Encoding, Page, ResultAllocator> result;
		result.reserve(left.size() + Encoding::max_units);
		result = left;
		result.append(1, right);
		return result;
	}

	template <typename Encoding, typename Page, typename RightAllocator, typename ResultAllocator = RightAllocator>
	basic_string<Encoding, Page, ResultAllocator> operator + (typename Encoding::point_type left, const basic_string<Encoding, Page, RightAllocator>& right)
	{
		basic_string<Encoding, Page, ResultAllocator> result;
		result.reserve(right.size() + Encoding::max_units);
		result.append(1, left);
		result.append(right);
		return result;
	}

	template <typename Encoding, typename Page, typename LeftAllocator, typename ResultAllocator = LeftAllocator,
		typename std::enable_if<lingo::utility::is_execution_set<Encoding, Page>::value, int>::type = 0>
	basic_string<Encoding, Page, ResultAllocator> operator + (const basic_string<Encoding, Page, LeftAllocator>& left, const typename Encoding::unit_type* right)
	{
		return left + basic_string_view<Encoding, Page>(right);
	}

	template <typename Encoding, typename Page, typename RightAllocator, typename ResultAllocator = RightAllocator,
		typename std::enable_if<lingo::utility::is_execution_set<Encoding, Page>::value, int>::type = 0>
	basic_string<Encoding, Page, ResultAllocator> operator + (const typename Encoding::unit_type* left, const basic_string<Encoding, Page, RightAllocator>& right)
	{
		return basic_string_view<Encoding, Page>(left) + right;
	}

	template <typename Encoding, typename Page,
		typename std::enable_if<lingo::utility::is_execution_set<Encoding, Page>::value, int>::type = 0>
	basic_string<Encoding, Page> operator + (basic_string_view<Encoding, Page> left, const typename Encoding::unit_type* right)
	{
		return left + basic_string_view<Encoding, Page>(right);
	}

	template <typename Encoding, typename Page,
		typename std::enable_if<lingo::utility::is_execution_set<Encoding, Page>::value, int>::type = 0>
	basic_string<Encoding, Page> operator + (const typename Encoding::unit_type* left, basic_string_view<Encoding, Page> right)
	{
		return basic_string_view<Encoding, Page>(left) + right;
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


	template <typename Encoding, typename Page, typename LeftAllocator,
		typename std::enable_if<lingo::utility::is_execution_set<Encoding, Page>::value, int>::type = 0>
	bool operator == (const basic_string<Encoding, Page, LeftAllocator>& left, const typename Encoding::unit_type* right) noexcept(noexcept(left.compare(right)))
	{
		return left.compare(right) == 0;
	}

	template <typename Encoding, typename Page, typename LeftAllocator,
		typename std::enable_if<lingo::utility::is_execution_set<Encoding, Page>::value, int>::type = 0>
	bool operator != (const basic_string<Encoding, Page, LeftAllocator>& left, const typename Encoding::unit_type* right) noexcept(noexcept(left.compare(right)))
	{
		return left.compare(right) != 0;
	}

	template <typename Encoding, typename Page, typename LeftAllocator,
		typename std::enable_if<lingo::utility::is_execution_set<Encoding, Page>::value, int>::type = 0>
	bool operator < (const basic_string<Encoding, Page, LeftAllocator>& left, const typename Encoding::unit_type* right) noexcept(noexcept(left.compare(right)))
	{
		return left.compare(right) < 0;
	}

	template <typename Encoding, typename Page, typename LeftAllocator,
		typename std::enable_if<lingo::utility::is_execution_set<Encoding, Page>::value, int>::type = 0>
	bool operator > (const basic_string<Encoding, Page, LeftAllocator>& left, const typename Encoding::unit_type* right) noexcept(noexcept(left.compare(right)))
	{
		return left.compare(right) > 0;
	}

	template <typename Encoding, typename Page, typename LeftAllocator,
		typename std::enable_if<lingo::utility::is_execution_set<Encoding, Page>::value, int>::type = 0>
	bool operator <= (const basic_string<Encoding, Page, LeftAllocator>& left, const typename Encoding::unit_type* right) noexcept(noexcept(left.compare(right)))
	{
		return left.compare(right) <= 0;
	}

	template <typename Encoding, typename Page, typename LeftAllocator,
		typename std::enable_if<lingo::utility::is_execution_set<Encoding, Page>::value, int>::type = 0>
	bool operator >= (const basic_string<Encoding, Page, LeftAllocator>& left, const typename Encoding::unit_type* right) noexcept(noexcept(left.compare(right)))
	{
		return left.compare(right) >= 0;
	}


	template <typename Encoding, typename Page, typename RightAllocator,
		typename std::enable_if<lingo::utility::is_execution_set<Encoding, Page>::value, int>::type = 0>
	bool operator == (const typename Encoding::unit_type* left, const basic_string<Encoding, Page, RightAllocator>& right) noexcept(noexcept(right.compare(left)))
	{
		return right.compare(left) == 0;
	}

	template <typename Encoding, typename Page, typename RightAllocator,
		typename std::enable_if<lingo::utility::is_execution_set<Encoding, Page>::value, int>::type = 0>
	bool operator != (const typename Encoding::unit_type* left, const basic_string<Encoding, Page, RightAllocator>& right) noexcept(noexcept(right.compare(left)))
	{
		return right.compare(left) != 0;
	}

	template <typename Encoding, typename Page, typename RightAllocator,
		typename std::enable_if<lingo::utility::is_execution_set<Encoding, Page>::value, int>::type = 0>
	bool operator < (const typename Encoding::unit_type* left, const basic_string<Encoding, Page, RightAllocator>& right) noexcept(noexcept(right.compare(left)))
	{
		return right.compare(left) > 0;
	}

	template <typename Encoding, typename Page, typename RightAllocator,
		typename std::enable_if<lingo::utility::is_execution_set<Encoding, Page>::value, int>::type = 0>
	bool operator > (const typename Encoding::unit_type* left, const basic_string<Encoding, Page, RightAllocator>& right) noexcept(noexcept(right.compare(left)))
	{
		return right.compare(left) < 0;
	}

	template <typename Encoding, typename Page, typename RightAllocator,
		typename std::enable_if<lingo::utility::is_execution_set<Encoding, Page>::value, int>::type = 0>
	bool operator <= (const typename Encoding::unit_type* left, const basic_string<Encoding, Page, RightAllocator>& right) noexcept(noexcept(right.compare(left)))
	{
		return right.compare(left) >= 0;
	}

	template <typename Encoding, typename Page, typename RightAllocator,
		typename std::enable_if<lingo::utility::is_execution_set<Encoding, Page>::value, int>::type = 0>
	bool operator >= (const typename Encoding::unit_type* left, const basic_string<Encoding, Page, RightAllocator>& right) noexcept(noexcept(right.compare(left)))
	{
		return right.compare(left) <= 0;
	}

	template <typename Unit, typename Allocator>
	typename std::enable_if<std::is_same<Unit, typename Allocator::value_type>::value, std::basic_ostream<Unit>&>::type 
		operator << (std::basic_ostream<Unit>& os,
			const lingo::basic_string<encoding::execution_encoding_t<Unit>, page::execution_page_t<Unit>, Allocator>& str)
	{
		return os.write(str.data(), static_cast<std::streamsize>(str.size()));
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
	using basic_utf8_le_string = basic_unicode_string<encoding::utf8_le<Unit, char32_t>, Allocator>;
	template <typename Unit, typename Allocator = internal::default_allocator<encoding::utf8<Unit, char32_t>>>
	using basic_utf8_be_string = basic_unicode_string<encoding::utf8_be<Unit, char32_t>, Allocator>;

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
	template <typename Allocator = internal::default_allocator<encoding::execution_encoding_t<char>>>
	using basic_narrow_string = basic_string<encoding::execution_encoding_t<char>, page::execution_page_t<char>, Allocator>;
	template <typename Allocator = internal::default_allocator<encoding::execution_encoding_t<wchar_t>>>
	using basic_wide_string = basic_string<encoding::execution_encoding_t<wchar_t>, page::execution_page_t<wchar_t>, Allocator>;

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