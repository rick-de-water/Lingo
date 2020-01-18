#ifndef H_LINGO_NULL_TERMINATED_STRING
#define H_LINGO_NULL_TERMINATED_STRING

#include <lingo/string.hpp>
#include <lingo/string_view.hpp>

#include <lingo/utility/object_builder.hpp>

#include <memory>

namespace lingo
{
	// Turns a string object into a null terminated string while trying to avoid copying data.
	// Only meant to be used when using an API that does not have a length parameter. 
	template <typename Encoding, typename Page, typename Allocator = internal::default_allocator<Encoding>>
	class basic_null_terminated_string
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

		using object_builder = utility::object_builder<unit_type>;

		private:
		using string = basic_string<encoding_type, page_type, allocator_type>;
		using string_view = basic_string_view<encoding_type, page_type>;

		public:
		basic_null_terminated_string() noexcept:
			_data(nullptr),
			_size(0)
		{
		}

		basic_null_terminated_string(const string& str) noexcept:
			_data(str.data()),
			_size(str.size())
		{
		}

		basic_null_terminated_string(string_view str) noexcept
		{
			_size = str.size();

			if (str.null_terminated())
			{
				_data = str.data();
			}
			else
			{
				_allocation = std::unique_ptr<unit_type[]>(new unit_type[str.size() + 1]);
				_data = _allocation.get();

				// TODO: use string_view::copy instead
				object_builder::copy_construct(_allocation.get(), str.data(), _size);

				LINGO_CONSTEXPR11 const unit_type null_terminator = {};
				object_builder::copy_construct(_allocation.get() + _size, &null_terminator, 1);
			}
		}

		const_pointer c_str() const noexcept
		{
			return _data;
		}

		operator string_view() const noexcept
		{
			return string_view(_data, _size, true);
		}

		private:
		std::unique_ptr<unit_type[]> _allocation;
		const_pointer _data;
		size_type _size;
	};

	// Fixed page typedefs
	template <typename Encoding, typename Allocator = internal::default_allocator<Encoding>>
	using basic_ascii_null_terminated_string = basic_null_terminated_string<Encoding, page::ascii, Allocator>;
	template <typename Encoding, typename Allocator = internal::default_allocator<Encoding>>
	using basic_unicode_null_terminated_string = basic_null_terminated_string<Encoding, page::unicode, Allocator>;

	// Fixed encoding typedefs
	template <typename Unit, typename Allocator = internal::default_allocator<encoding::utf8<Unit, char32_t>>>
	using basic_utf8_null_terminated_string = basic_unicode_null_terminated_string<encoding::utf8<Unit, char32_t>, Allocator>;

	template <typename Unit, typename Allocator = internal::default_allocator<encoding::utf8<Unit, char32_t>>>
	using basic_utf16_null_terminated_string = basic_unicode_null_terminated_string<encoding::utf16<Unit, char32_t>, Allocator>;
	template <typename Unit, typename Allocator = internal::default_allocator<encoding::utf8<Unit, char32_t>>>
	using basic_utf16_le_null_terminated_string = basic_unicode_null_terminated_string<encoding::utf16_le<Unit, char32_t>, Allocator>;
	template <typename Unit, typename Allocator = internal::default_allocator<encoding::utf8<Unit, char32_t>>>
	using basic_utf16_be_null_terminated_string = basic_unicode_null_terminated_string<encoding::utf16_be<Unit, char32_t>, Allocator>;

	template <typename Unit, typename Allocator = internal::default_allocator<encoding::none<Unit, char32_t>>>
	using basic_utf32_null_terminated_string = basic_unicode_null_terminated_string<encoding::utf32<Unit, char32_t>, Allocator>;
	template <typename Unit, typename Allocator = internal::default_allocator<encoding::none<Unit, char32_t>>>
	using basic_utf32_le_null_terminated_string = basic_unicode_null_terminated_string<encoding::utf32_le<Unit, char32_t>, Allocator>;
	template <typename Unit, typename Allocator = internal::default_allocator<encoding::none<Unit, char32_t>>>
	using basic_utf32_be_null_terminated_string = basic_unicode_null_terminated_string<encoding::utf32_be<Unit, char32_t>, Allocator>;

	// Fixed encoding & page typedefs
	template <typename Allocator = internal::default_allocator<encoding::execution_encoding_t<char>>>
	using basic_narrow_null_terminated_string = basic_null_terminated_string<encoding::execution_encoding_t<char>, page::execution_page_t<char>, Allocator>;
	template <typename Allocator = internal::default_allocator<encoding::execution_encoding_t<wchar_t>>>
	using basic_wide_null_terminated_string = basic_null_terminated_string<encoding::execution_encoding_t<wchar_t>, page::execution_page_t<wchar_t>, Allocator>;

	// Fully specialized typedefs
	using narrow_null_terminated_string = basic_narrow_null_terminated_string<>;
	using wide_null_terminated_string = basic_wide_null_terminated_string<>;

	using ascii_null_terminated_string = basic_ascii_null_terminated_string<encoding::none<char, char>>;

	#ifdef __cpp_char8_t
	using utf8_null_terminated_string = basic_utf8_null_terminated_string<char8_t>;
	#else
	using utf8_null_terminated_string = basic_utf8_null_terminated_string<char>;
	#endif

	using utf16_null_terminated_string = basic_utf16_null_terminated_string<char16_t>;
	using utf16_le_null_terminated_string = basic_utf16_le_null_terminated_string<char16_t>;
	using utf16_be_null_terminated_string = basic_utf16_be_null_terminated_string<char16_t>;

	using utf32_null_terminated_string = basic_utf32_null_terminated_string<char32_t>;
	using utf32_le_null_terminated_string = basic_utf32_le_null_terminated_string<char32_t>;
	using utf32_be_null_terminated_string = basic_utf32_be_null_terminated_string<char32_t>;

	// Default string typedef
	using null_terminated_string = utf8_null_terminated_string;
}

#endif