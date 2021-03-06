#ifndef H_LINGO_ENCODING_POINT_ITERATOR
#define H_LINGO_ENCODING_POINT_ITERATOR

#include <lingo/platform/constexpr.hpp>
#include <lingo/error/exception.hpp>

#include <lingo/utility/compressed_pair.hpp>

#include <iterator>

namespace lingo
{
	template <typename Encoding, typename Page, typename Allocator>
	class basic_string;

	template <typename Encoding, typename Page>
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
			using value_type = point_type;
			using pointer = value_type*;
			using const_pointer = const value_type*;
			using reference = value_type&;
			using const_reference = const value_type&;

			LINGO_CONSTEXPR14 point_iterator() noexcept:
				_current(nullptr),
				_end(nullptr),
				_last(nullptr),
				_state{}
			{
			}

			template <typename Page, typename Allocator>
			point_iterator(const basic_string<encoding_type, Page, Allocator>& str):
				_current(str.data()),
				_end(str.data() + str.size()),
				_last(str.data()),
				_state{}
			{
				parse_next();
			}

			template <typename Page>
			LINGO_CONSTEXPR14 point_iterator(basic_string_view<encoding_type, Page> str):
				_current(str.data()),
				_end(str.data() + str.size()),
				_last(str.data()),
				_state{}
			{
				parse_next();
			}

			LINGO_CONSTEXPR14 const unit_type* read_ptr() const noexcept
			{
				return _last;
			}

			LINGO_CONSTEXPR14 const_reference operator * () const noexcept
			{
				return _state.first();
			}

			LINGO_CONSTEXPR14 point_iterator& operator ++()
			{
				parse_next();
				return *this;
			}

			LINGO_CONSTEXPR14 point_iterator operator ++(int)
			{
				point_iterator prev(*this);
				operator++();
				return prev;
			}

			LINGO_CONSTEXPR14 const_pointer operator -> () const noexcept
			{
				return &(_state.first());
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
			LINGO_CONSTEXPR14 void parse_next()
			{
				// End of string reached
				if (_current == _end)
				{
					_current = nullptr;
					_end = nullptr;
					_last = nullptr;
					return;
				}

				const utility::span<const unit_type> source_span(_current, _end);
				const utility::span<point_type> destination_span(&(_state.first()), 1);

				// Decode the next points
				auto result = encoding_type::decode_one(source_span, destination_span, _state.second(), true);
				// TODO: configurable error handling
				if (result.error != error::error_code::success)
				{
					throw error::exception(result.error);
				}

				// Move the current pointer
				_last = _current;
				_current += source_span.diff(result.source).size();
				assert(_current <= _end); // _current should never go beyond _end
			}

			const unit_type* _current;
			const unit_type* _end;
			const unit_type* _last;
			utility::compressed_pair<point_type, typename encoding_type::decode_state_type> _state;
		};

		template <typename Encoding>
		LINGO_CONSTEXPR14 point_iterator<Encoding> begin(const point_iterator<Encoding>& it)
		{
			return it;
		}

		template <typename Encoding>
		LINGO_CONSTEXPR14 point_iterator<Encoding> end(const point_iterator<Encoding>&)
		{
			return {};
		}
	}
}

#endif