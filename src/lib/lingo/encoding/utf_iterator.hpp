#ifndef H_LINGO_ENCODING_UTFITERATOR
#define H_LINGO_ENCODING_UTFITERATOR

#include <lingo/standard_typedefs.hpp>
#include <lingo/string_view.hpp>

#include <lingo/encoding/utf8.hpp>

#include <assert.h>
#include <cstddef>
#include <exception>
#include <iterator>
#include <limits>
#include <type_traits>

namespace lingo
{
	namespace encoding
	{
		template <typename UnitT, std::enable_if<sizeof(UnitT) == 1, int>::type = 0>
		LINGO_CONSTEXPR11 std::size_t utf_code_point_units(UnitT unit) noexcept
		{
			LINGO_CONSTEXPR11 std::size_t invalid = 0;

			LINGO_CONSTEXPR11 std::size_t sizes[]
			{
				// First 128 bytes are single byte code points
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

				// Continuation bytes are invalid
				invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid,
				invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid,
				invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid,
				invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid,

				// 2 byte code points
				2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
				2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,

				// 3 byte code points
				3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,

				// 4 byte code points
				4, 4, 4, 4, 4, 4, 4, 4,

				// Code points longer than 4 bytes are invalid
				invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid
			};

			return sizes[-static_cast<int_fast16_t>(std::numeric_limits<UnitT>::min()) + static_cast<int_fast16_t>(unit)];
		}

		template <typename UnitT, std::enable_if<sizeof(UnitT) == 2, int>::type = 0>
		LINGO_CONSTEXPR11 std::size_t utf_code_point_units(UnitT unit) noexcept
		{
			return ((unit & 0xFC00) == 0xD800) ? 2 : 1;
		}

		template <typename UnitT, std::enable_if<sizeof(UnitT) == 4, int>::type = 0>
		LINGO_CONSTEXPR11 std::size_t utf_code_point_units(UnitT unit) noexcept
		{
			return 1;
		}

		template <typename UnitT, typename PointT = char32_t, std::enable_if<sizeof(UnitT) == 1, int>::type = 0>
		LINGO_CONSTEXPR14 PointT utf_code_point(const UnitT* units, std::size_t count)
		{
			// Check if there is at least one unit
			if (count == 0)
			{
				throw std::range_error("Not enough code units for next code point");
			}

			// Calculate the size of the code point
			const std::size_t code_point_units = utf_code_point_units(*units);

			// Check if the first code unit is valid
			if (code_point_units == 0)
			{
				throw std::range_error("Invalid code point");
			}
			assert(code_point_units <= 4);

			// Check if there are enough code units left for the code point
			if (count < code_point_units)
			{
				throw std::range_error("Not enough code units for next code point");
			}

			// Mask constants
			LINGO_CONSTEXPR11 UnitT first_data_masks[] = { UnitT(0b1111'1111), UnitT(0b1111'1111), UnitT(0b0001'1111), UnitT(0b0000'1111), UnitT(0b0000'0111) };
			LINGO_CONSTEXPR11 UnitT continuation_data_mask = UnitT(0b0011'1111);
			LINGO_CONSTEXPR11 UnitT continuation_prefix_mask = UnitT(0b1100'0000);
			LINGO_CONSTEXPR11 UnitT continuation_bits = UnitT(0b1000'0000);

			// Get the bits from the first unit
			PointT code_point = units[0] & first_data_masks[code_point_units];

			// Get the bits from all the continuation bytes
			for (std::size_t i = 1; i < code_point_units; ++i)
			{
				// Every continuation byte adds 6 bits of information, so we shift the code point to the left by 6
				code_point <<= 6;

				// Check if this is a valid continuation byte
				if ((units[i] & continuation_prefix_mask) != continuation_bits)
				{
					throw std::range_error("Invalid continuation byte");
				}

				// Add the bits to the code point
				code_point |= units[i] & continuation_data_mask;
			}

			// Return the result
			return code_point;
		}

		template <typename UnitT, typename PointT = char32_t, std::enable_if<sizeof(UnitT) == 2, int>::type = 0>
		LINGO_CONSTEXPR14 PointT utf_code_point(const UnitT* units, std::size_t count)
		{
			// Check if there is at least one unit
			if (count == 0)
			{
				throw std::range_error("Not enough code units for next code point");
			}

			// Calculate the size of the code point
			const std::size_t code_point_units = utf_code_point_units(*units);

			// Check if the first code unit is valid
			if (code_point_units == 0)
			{
				throw std::range_error("Invalid code point");
			}
			assert(code_point_units <= 2);

			// Check if there are enough code units left for the code point
			if (count < code_point_units)
			{
				throw std::range_error("Not enough code units for next code point");
			}

			// Single unit, just cast to PointT
			if (code_point_units == 1)
			{
				return static_cast<PointT>(*units);
			}
			// Combine 10 bit values from high and low surrogate
			else
			{
				// Should always be 2
				assert(code_point_units == 2);

				// Check if the low surrogate is valid
				if ((units[1] & 0xFC00) != 0xDC00)
				{
					throw std::range_error("Invalid low surrogate");
				}

				// Combine the surrogates to a single code point
				return PointT(((units[0] & 0x3FF) << 10) | (units[1] & 0x3FF));
			}
		}

		template <typename UnitT, typename PointT = char32_t, std::enable_if<sizeof(UnitT) == 4, int>::type = 0>
		LINGO_CONSTEXPR14 PointT utf_code_point(const UnitT* units, std::size_t count)
		{
			// Check if there is at least one unit
			if (count == 0)
			{
				throw std::range_error("Not enough code units for next code point");
			}

			return static_cast<PointT>(*units);
		}

		template <typename UnitT, typename PointT = char32_t>
		class utf_iterator : public standard_typedefs<PointT>
		{
			static_assert(std::is_integral_v<UnitT>, "Unit type must be integral");
			static_assert(std::is_integral_v<PointT>, "Point type must be integral");
			static_assert(sizeof(PointT) >= 4, "Point type must be at least 4 bytes in size");

			public:
			using iterator_catagory = std::input_iterator_tag;

			LINGO_CONSTEXPR14 utf_iterator() noexcept = default;

			LINGO_CONSTEXPR14 utf_iterator() noexcept:
				_current(nullptr),
				_end(nullptr),
				_code_point(0)
			{
			}

			template <typename CharacterSet>
			LINGO_CONSTEXPR14 utf_iterator(basic_string_view<UnitT, utf8<UnitT>, CharacterSet> string) noexcept:
				_current(string.data()),
				_end(string.data() + string.size()),
				_code_point(parse())
			{
			}

			LINGO_CONSTEXPR14 utf_iterator& operator ++ ()
			{
				// Shift read pointer
				const std::size_t code_point_units = utf_code_point_units(*_current);
				assert(code_point_units > 0 && code_point_units <= 4); // Code point parsing should have already filtered out all invalid code units
				assert(_current != nullptr);
				_current += code_point_units;

				// Parse the next code point
				_code_point = parse();

				// Return this
				return *this;
			}

			LINGO_CONSTEXPR14 utf_iterator operator ++ (int)
			{
				// Copy this in its current state
				auto old = *this;

				// Increment this
				++(*this);

				// Return the copy of the previous state
				return old;
			}

			LINGO_CONSTEXPR14 const_reference operator * () const noexcept
			{
				return _code_point;
			}

			LINGO_CONSTEXPR14 const_pointer operator -> () const noexcept
			{
				return &_code_point;
			}

			LINGO_CONSTEXPR14 bool operator == (const utf_iterator& right) const noexcept
			{
				return (_current == right._current) && (_end == right._end);
			}

			LINGO_CONSTEXPR14 bool operator != (const utf_iterator& right) const noexcept
			{
				return !(*this == right);
			}

			private:
			LINGO_CONSTEXPR14 value_type parse()
			{
				// End of string reached
				if (_current == _end)
				{
					_current = nullptr;
					_end = nullptr;
					return 0;
				}

				// _current should never go beyond _end
				assert(_current < _end);

				// Parse the code point
				return utf_code_point(_current, _end - _current);
			}

			const UnitT* _current;
			const UnitT* _end;
			value_type _code_point;
		};

		template <typename UnitT>
		LINGO_CONSTEXPR14 utf_iterator<UnitT> begin(utf_iterator<UnitT> iterator) noexcept
		{
			return iterator;
		}

		template <typename UnitT>
		LINGO_CONSTEXPR14 utf_iterator<UnitT> end(const utf_iterator<UnitT>&) noexcept
		{
			return {};
		}
	}
}

#endif