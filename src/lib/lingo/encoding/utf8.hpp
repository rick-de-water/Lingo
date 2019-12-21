#ifndef H_LINGO_ENCODING_UTF8
#define H_LINGO_ENCODING_UTF8

#include <lingo/constexpr.hpp>

#include <lingo/encoding/result.hpp>
#include <lingo/encoding/point_iterator.hpp>

#include <cassert>
#include <climits>
#include <limits>
#include <type_traits>

namespace lingo
{
	namespace encoding
	{
		template <typename UnitT, typename PointT>
		struct utf8
		{
			public:
			using unit_type = UnitT;
			using point_type = PointT;

			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;

			private:
			static_assert(sizeof(unit_type) * CHAR_BIT >= 8, "A single unit must have at least 8 bits");

			static constexpr bool unit_has_8_bit_maginude = std::numeric_limits<unit_type>::digits >= 8;

			static constexpr bool point_has_7_bit_maginude = std::numeric_limits<point_type>::digits >= 7;
			static constexpr bool point_has_11_bit_maginude = std::numeric_limits<point_type>::digits >= 11;
			static constexpr bool point_has_16_bit_maginude = std::numeric_limits<point_type>::digits >= 16;
			static constexpr bool point_has_21_bit_maginude = std::numeric_limits<point_type>::digits >= 21;

			static LINGO_CONSTEXPR11 unit_type point_size_markers[4] =
			{
				static_cast<unit_type>(0),
				static_cast<unit_type>(unit_has_8_bit_maginude ? 192 : -64),
				static_cast<unit_type>(unit_has_8_bit_maginude ? 224 : -32),
				static_cast<unit_type>(unit_has_8_bit_maginude ? 240 : -16)
			};

			static LINGO_CONSTEXPR11 unit_type continuation_unit_marker = static_cast<unit_type>(unit_has_8_bit_maginude ? 128 : -128);
			static LINGO_CONSTEXPR11 unit_type continuation_unit_data_mask = static_cast<unit_type>(63);

			public:
			static LINGO_CONSTEXPR11 size_type max_point_size = 4;

			static LINGO_CONSTEXPR14 size_type point_size(point_type point) noexcept
			{
				// Check if the point is too big to be encoded
				LINGO_IF_CONSTEXPR(std::is_signed<point_type>::value && !point_has_21_bit_maginude)
				{
					if (point < -(0x110000 / 2) || point >= (0x110000 / 2))
					{
						return 0;
					}
				}
				else
				{
					if (point >= 0x110000)
					{
						return 0;
					}
				}

				// Calculate the actual code point
				char32_t code_point;
				LINGO_IF_CONSTEXPR(std::is_signed<point_type>::value)
				{
					if (point >= 0)
					{
						code_point = static_cast<char32_t>(point);
					}
					else
					{
						code_point = static_cast<int_least32_t>(point);
						code_point += -static_cast<std::int_fast16_t>(std::numeric_limits<point_type>::min()) * 2;
					}
				}
				else
				{
					code_point = static_cast<char32_t>(point);
				}
				assert(code_point < 0x110000);

				// Calculate size
				if (code_point < 0x80)
				{
					return 1;
				}
				else if (code_point < 0x800)
				{
					return 2;
				}
				else if (code_point < 0x10000)
				{
					return 3;
				}
				else
				{
					return 4;
				}
			}

			static LINGO_CONSTEXPR14 size_type unit_size(unit_type unit) noexcept
			{
				LINGO_CONSTEXPR11 size_type sizes[]
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
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

					// 2 byte code points
					2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
					2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,

					// 3 byte code points
					3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,

					// 4 byte code points
					4, 4, 4, 4, 4, 4, 4, 4,

					// Code points longer than 4 bytes are invalid
					0, 0, 0, 0, 0, 0, 0, 0
				};

				LINGO_IF_CONSTEXPR(sizeof(unit_type) * CHAR_BIT > 8)
				{
					if (unit < 0 || unit > 255)
					{
						return 0;
					}
				}

				LINGO_IF_CONSTEXPR(!unit_has_8_bit_maginude)
				{
					if (unit >= 0)
					{
						return sizes[unit];
					}
					else
					{
						difference_type offset = static_cast<difference_type>(std::numeric_limits<unit_type>::min()) * 2LL;
						return sizes[static_cast<difference_type>(unit) - offset];
					}
				}
				else
				{
					return sizes[unit];
				}
			}

			static LINGO_CONSTEXPR14 encode_result encode_point(point_type point, unit_type* buffer, size_type buffer_size) noexcept
			{
				// Calculate the number of units needed to encode this point
				const size_type required_size = point_size(point);
				if (required_size == 0)
				{
					return { 0, error::error_code::invalid_point };
				}
				assert(required_size <= max_point_size);

				// Check if the buffer is large enough to fit the encoded point
				if (required_size > buffer_size)
				{
					return { 0, error::error_code::buffer_too_small };
				}

				// Encode the first unit
				const size_type last_index = required_size - 1;
				buffer[0] = static_cast<unit_type>(point_size_markers[last_index] | (point >> (6 * last_index)));

				// Encode the subsequent units
				for (size_t i = 1; i < required_size; ++i)
				{
					buffer[i] = static_cast<unit_type>(continuation_unit_marker | ((point >> (6 * (last_index - i))) & continuation_unit_data_mask));
				}

				// Return the result
				return { required_size, error::error_code::success };
			}

			static LINGO_CONSTEXPR14 decode_result<point_type> decode_point(const unit_type* buffer, size_type buffer_size) noexcept
			{
				// Check if there is at least one unit
				if (buffer_size == 0)
				{
					return { 0, error::error_code::buffer_too_small };
				}

				// Calculate the size of the code point
				const std::size_t code_point_units = point_size(*buffer);

				// Check if the first code unit is valid
				if (code_point_units == 0)
				{
					return { 0, error::error_code::invalid_first_unit };
				}
				assert(code_point_units <= 4);

				// Check if there are enough code units left for the code point
				if (buffer_size < code_point_units)
				{
					return { 0, error::error_code::buffer_too_small };
				}

				// Mask constants
				LINGO_CONSTEXPR11 UnitT first_data_masks[] = { UnitT(0b1111'1111), UnitT(0b1111'1111), UnitT(0b0001'1111), UnitT(0b0000'1111), UnitT(0b0000'0111) };
				LINGO_CONSTEXPR11 UnitT continuation_data_mask = UnitT(0b0011'1111);
				LINGO_CONSTEXPR11 UnitT continuation_prefix_mask = UnitT(0b1100'0000);
				LINGO_CONSTEXPR11 UnitT continuation_bits = UnitT(0b1000'0000);

				// Get the bits from the first unit
				PointT code_point = buffer[0] & first_data_masks[code_point_units];

				// Get the bits from all the continuation bytes
				for (std::size_t i = 1; i < code_point_units; ++i)
				{
					// Every continuation byte adds 6 bits of information, so we shift the code point to the left by 6
					code_point <<= 6;

					// Check if this is a valid continuation byte
					if ((buffer[i] & continuation_prefix_mask) != continuation_bits)
					{
						return { 0, error::error_code::invalid_subsequent_unit };
					}

					// Add the bits to the code point
					code_point |= buffer[i] & continuation_data_mask;
				}

				// Return the result
				return { code_point, error::error_code::success };
			}
		};

		template <typename UnitT, typename PointT>
		LINGO_CONSTEXPR11 UnitT utf8<UnitT, PointT>::point_size_markers[4];

		template <typename UnitT, typename PointT>
		using basic_utf8_point_iterator = lingo::encoding::point_iterator<utf8<UnitT, PointT>>;
	}
}

#endif