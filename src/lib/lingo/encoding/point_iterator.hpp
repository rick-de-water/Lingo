#ifndef H_LINGO_ENCODING_POINT_ITERATOR
#define H_LINGO_ENCODING_POINT_ITERATOR

#include <lingo/constexpr.hpp>
#include <lingo/error/exception.hpp>

#include <iterator>

namespace lingo
{
	template <typename Encoding, typename CharacterSet>
	class basic_string_view;

	namespace encoding
	{
		template <typename Encoding>
		class point_iterator
		{
			public:
			using encoding_type = Encoding;

			using unit_type = typename encoding_type::unit_type;
			using point_type = typename encoding_type::point_type;

			using size_type = typename encoding_type::size_type;
			using difference_type = typename encoding_type::difference_type;

			using iterator_catagory = std::input_iterator_tag;
			using value_type = unit_type;
			using pointer = value_type*;
			using const_pointer = const value_type*;
			using reference = value_type&;
			using const_reference = const value_type&;

			LINGO_CONSTEXPR14 point_iterator() noexcept:
				_current(nullptr),
				_end(nullptr),
				_code_point(0)
			{
			}

			template <typename CharacterSet>
			LINGO_CONSTEXPR14 point_iterator(basic_string_view<encoding_type, CharacterSet> string) noexcept:
				_current(string.data()),
				_end(string.data() + string.size()),
				_code_point(parse())
			{
			}

			LINGO_CONSTEXPR14 const_reference operator * () const noexcept
			{
				return _code_point;
			}

			LINGO_CONSTEXPR14 const_pointer operator -> () const noexcept
			{
				return &_code_point;
			}

			LINGO_CONSTEXPR14 bool operator == (const point_iterator& right) const noexcept
			{
				return (_current == right._current) && (_end == right._end);
			}

			LINGO_CONSTEXPR14 bool operator != (const point_iterator& right) const noexcept
			{
				return !(*this == right);
			}

			private:
			LINGO_CONSTEXPR14 point_type parse()
			{
				// End of string reached
				if (_current == _end)
				{
					_current = nullptr;
					_end = nullptr;
					return 0;
				}

				auto result = encoding_type::decode_point(_current, _end - _current);
				// TODO: configurable error handling
				if (result.error != error::error_code::success)
				{
					throw error::exception(result.error);
				}

				// _current should never go beyond _end
				assert(_current < _end);

				// Parse the code point
				return utf_code_point(_current, _end - _current);
			}

			const unit_type* _current;
			const unit_type* _end;
			point_type _code_point;
		};
	}
}

#endif