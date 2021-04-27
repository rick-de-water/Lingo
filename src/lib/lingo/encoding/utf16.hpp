#ifndef H_LINGO_ENCODING_UTF16
#define H_LINGO_ENCODING_UTF16

#include <lingo/platform/constexpr.hpp>

#include <lingo/encoding/result.hpp>
#include <lingo/encoding/internal/bit_converter.hpp>

#include <cassert>
#include <cstring>
#include <climits>
#include <limits>
#include <type_traits>

namespace lingo
{
	namespace encoding
	{
		template <typename Unit, typename Point>
		struct utf16
		{
			public:
			using unit_type = Unit;
			using point_type = Point;

			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;

			static LINGO_CONSTEXPR11 size_type max_units = 2;
			static LINGO_CONSTEXPR11 size_type min_unit_bits = 16;
			static LINGO_CONSTEXPR11 size_type min_point_bits = 21;

			using encode_result_type = encode_result<unit_type, point_type>;
			using decode_result_type = decode_result<unit_type, point_type>;
			using encode_source_type = typename encode_result_type::source_type;
			using decode_source_type = typename decode_result_type::source_type;
			using encode_destination_type = typename encode_result_type::destination_type;
			using decode_destination_type = typename decode_result_type::destination_type;

			struct encode_state_type {};
			struct decode_state_type {};

			private:
			using bit_converter_type = internal::bit_converter<unit_type, min_unit_bits, point_type, min_point_bits>;
			using unit_bits_type = typename bit_converter_type::unit_bits_type;
			using point_bits_type = typename bit_converter_type::point_bits_type;

			public:
			static LINGO_CONSTEXPR14 size_type point_size(point_type point) noexcept
			{
				// Convert to bits
				const point_bits_type point_bits = bit_converter_type::to_point_bits(point);

				// Reject values above 0x10FFFF
				LINGO_IF_CONSTEXPR(sizeof(point_bits_type) * CHAR_BIT > min_point_bits)
				{
					if (point_bits > 0x10FFFF)
					{
						return 0;
					}
				}

				// Multi unit encoding
				if (point_bits > 0xFFFF)
				{
					return 2;
				}
				else
				{
					// Reject surrogates
					if ((point_bits & 0xF800) == 0xD800)
					{
						return 0;
					}

					// Single unit encoding
					return 1;
				}
			}

			static LINGO_CONSTEXPR14 size_type unit_size(unit_type unit) noexcept
			{
				// Convert to bits
				const unit_bits_type unit_bits = bit_converter_type::to_unit_bits(unit);

				// Reject values above 0xFFFF
				LINGO_IF_CONSTEXPR(sizeof(unit_bits_type) * CHAR_BIT > min_unit_bits)
				{
					if (unit_bits > 0xFFFF)
					{
						return 0;
					}
				}

				// Check for surrogate code unit
				if ((unit_bits & 0xF800) == 0xD800)
				{
					// High surrogate
					if ((unit_bits & 0x0400) == 0)
					{
						return 2;
					}
					// Reject low surrogate
					else
					{
						return 0;
					}
				}
				// No surrogate
				else
				{
					return 1;
				}
			}

			static LINGO_CONSTEXPR14 encode_result_type encode_one(encode_source_type source, encode_destination_type destination, encode_state_type&, bool) noexcept
			{
				return encode_one(source, destination);
			}

			static LINGO_CONSTEXPR14 encode_result_type encode_one(encode_source_type source, encode_destination_type destination) noexcept
			{
				// Check if there is at least 1 point
				if (source.size() < 1)
				{
					return { source, destination, error::error_code::source_buffer_too_small };
				}

				// Calculate the number of units needed to encode this point
				const size_type required_size = point_size(source[0]);
				if (required_size == 0)
				{
					return { source, destination, error::error_code::invalid_point };
				}
				assert(required_size <= max_units);

				// Check if the destination buffer is large enough to fit the encoded point
				if (required_size > destination.size())
				{
					return { source, destination, error::error_code::destination_buffer_too_small };
				}

				// Convert to bits
				point_bits_type point_bits = bit_converter_type::to_point_bits(source[0]);

				// Encode single unit
				if (required_size == 1)
				{
					const unit_bits_type unit_bits = static_cast<unit_bits_type>(point_bits);
					destination[0] = bit_converter_type::from_unit_bits(unit_bits);
					return { source.subspan(1), destination.subspan(1), error::error_code::success };
				}
				// Encode surrogate pair
				else
				{
					assert(required_size == 2);

					point_bits -= 0x10000;
					const unit_bits_type high_unit_bits = static_cast<unit_bits_type>((point_bits >> 10) + 0xD800);
					const unit_bits_type low_unit_bits = static_cast<unit_bits_type>((point_bits & 0x3FF) + 0xDC00);

					destination[0] = bit_converter_type::from_unit_bits(high_unit_bits);
					destination[1] = bit_converter_type::from_unit_bits(low_unit_bits);

					return { source.subspan(1), destination.subspan(2), error::error_code::success };
				}
			}

			static LINGO_CONSTEXPR14 decode_result_type decode_one(decode_source_type source, decode_destination_type destination, decode_state_type&, bool) noexcept
			{
				return decode_one(source, destination);
			}

			static LINGO_CONSTEXPR14 decode_result_type decode_one(decode_source_type source, decode_destination_type destination) noexcept
			{
				// Check if there is at least one unit
				if (source.size() < 1)
				{
					return { source, destination, error::error_code::source_buffer_too_small };
				}

				// Calculate the size of the code point
				const std::size_t required_size = unit_size(source.front());

				// Check if the first code unit is valid
				if (required_size == 0)
				{
					return { source, destination, error::error_code::invalid_unit };
				}
				assert(required_size <= max_units);

				// Check if there are enough code units left for the code point
				if (source.size() < required_size)
				{
					return { source, destination, error::error_code::source_buffer_too_small };
				}

				// Check if there is room for at least 1 point
				if (destination.size() < 1)
				{
					return { source, destination, error::error_code::destination_buffer_too_small };
				}

				// Decode single unit
				const unit_bits_type high_unit_bits = bit_converter_type::to_unit_bits(source[0]);
				if (required_size == 1)
				{
					const point_bits_type point_bits = static_cast<point_bits_type>(high_unit_bits);
					destination[0] = bit_converter_type::from_point_bits(point_bits);
					return { source.subspan(1), destination.subspan(1), error::error_code::success };
				}
				// Decode surrogate pair
				else
				{
					assert(required_size == 2);
					const unit_bits_type low_unit_bits = bit_converter_type::to_unit_bits(source[1]);

					// Validate low surrogate
					if ((low_unit_bits & 0xFC00) != 0xDC00)
					{
						return { source, destination, error::error_code::invalid_unit };
					}

					point_bits_type point_bits = static_cast<point_bits_type>(high_unit_bits - 0xD800);
					point_bits <<= 10;
					point_bits |= static_cast<point_bits_type>(low_unit_bits - 0xDC00);
					point_bits += 0x10000;

					destination[0] = bit_converter_type::from_point_bits(point_bits);
					return { source.subspan(2), destination.subspan(1), error::error_code::success };
				}
			}
		};

		template <typename Unit, typename Point>
		LINGO_CONSTEXPR11 typename utf16<Unit, Point>::size_type utf16<Unit, Point>::max_units;
		template <typename Unit, typename Point>
		LINGO_CONSTEXPR11 typename utf16<Unit, Point>::size_type utf16<Unit, Point>::min_unit_bits;
		template <typename Unit, typename Point>
		LINGO_CONSTEXPR11 typename utf16<Unit, Point>::size_type utf16<Unit, Point>::min_point_bits;
	}
}

#include <lingo/encoding/endian.hpp>
#include <lingo/encoding/join.hpp>

namespace lingo
{
	namespace encoding
	{
		template <typename Unit, typename Point>
		using utf16_se = join<swap_endian<Unit>, utf16<Unit, Point>>;

		#if LINGO_ARCHITECTURE_ENDIANNESS == LINGO_ARCHITECTURE_LITTLE_ENDIAN
		template <typename Unit, typename Point>
		using utf16_le = utf16<Unit, Point>;
		template <typename Unit, typename Point>
		using utf16_be = utf16_se<Unit, Point>;
		#else
		template <typename Unit, typename Point>
		using utf16_le = utf16_se<Unit, Point>;
		template <typename Unit, typename Point>
		using utf16_be = utf16<Unit, Point>;
		#endif
	}
}

#endif