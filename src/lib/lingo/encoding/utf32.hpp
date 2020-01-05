#ifndef H_LINGO_ENCODING_UTF32
#define H_LINGO_ENCODING_UTF32

#include <lingo/platform/constexpr.hpp>

#include <lingo/encoding/endian.hpp>
#include <lingo/encoding/result.hpp>
#include <lingo/encoding/internal/bit_converter.hpp>

#include <climits>
#include <cstddef>

namespace lingo
{
	namespace encoding
	{
		template <typename Unit, typename Point>
		struct utf32
		{
			using unit_type = Unit;
			using point_type = Point;

			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;

			static LINGO_CONSTEXPR11 size_type max_units = 1;
			static LINGO_CONSTEXPR11 size_type min_unit_bits = 21;
			static LINGO_CONSTEXPR11 size_type min_point_bits = 21;

			using bit_converter = internal::bit_converter<unit_type, min_unit_bits, point_type, min_point_bits>;
			using unit_bits_type = typename bit_converter::unit_bits_type;
			using point_bits_type = typename bit_converter::point_bits_type;

			static LINGO_CONSTEXPR14 encode_result encode_point(point_type point, unit_type* buffer, size_type buffer_size) noexcept
			{
				// Reject values below zero when there are enough magnitude bits
				LINGO_IF_CONSTEXPR(std::is_signed<point_type>::value && std::numeric_limits<point_type>::digits > min_point_bits)
				{
					if (point < 0)
					{
						return { 0, error::error_code::invalid_point };
					}
				}

				// Convert to bits
				const point_bits_type point_bits = bit_converter::to_point_bits(point);

				// Reject points beyond 0x10FFFF
				LINGO_IF_CONSTEXPR(sizeof(point_bits_type) * CHAR_BIT > min_point_bits)
				{
					if (point_bits > 0x10FFFF)
					{
						return { 0, error::error_code::invalid_point };
					}
				}

				// Reject surrogates
				if ((point_bits >= 0xD800 && point_bits < 0xE000))
				{
					return { 0, error::error_code::invalid_point };
				}

				// Check if the buffer is large enough
				if (buffer_size == 0)
				{
					return { 1, error::error_code::buffer_too_small };
				}

				// Return result
				const unit_bits_type unit_bits = static_cast<unit_bits_type>(point_bits);
				const unit_type unit = bit_converter::from_unit_bits(unit_bits);
				buffer[0] = unit;
				return { 1, error::error_code::success };
			}

			static LINGO_CONSTEXPR14 decode_result<point_type> decode_point(const unit_type* buffer, size_type buffer_size) noexcept
			{
				// Check if the buffer is large enough
				if (buffer_size == 0)
				{
					return { 0, 0, error::error_code::buffer_too_small };
				}

				// Reject values below zero when there are enough magnitude bits
				LINGO_IF_CONSTEXPR(std::is_signed<point_type>::value && std::numeric_limits<point_type>::digits > min_point_bits)
				{
					if (buffer[0] < 0)
					{
						return { 0, 0, error::error_code::invalid_unit };
					}
				}

				// Convert to bits
				const unit_bits_type unit_bits = bit_converter::to_unit_bits(buffer[0]);

				// Reject points beyond 0x10FFFF
				LINGO_IF_CONSTEXPR(std::numeric_limits<unit_bits_type>::digits > min_unit_bits)
				{
					if (unit_bits > 0x10FFFF)
					{
						return { 0, 0, error::error_code::invalid_unit };
					}
				}

				// Reject surrogates
				if ((unit_bits >= 0xD800 && unit_bits < 0xE000))
				{
					return { 0, 0, error::error_code::invalid_unit };
				}

				// Return result
				const point_bits_type point_bits = static_cast<point_bits_type>(unit_bits);
				const point_type point = bit_converter::from_point_bits(point_bits);
				return { point, 1, error::error_code::success };
			}
		};

		#if LINGO_ARCHITECTURE_ENDIANNESS == LINGO_ARCHITECTURE_LITTLE_ENDIAN
		template <typename Unit, typename Point>
		using utf32_le = utf32<Unit, Point>;
		template <typename Unit, typename Point>
		using utf32_be = swap_endian<utf32<Unit, Point>>;
		#else
		template <typename Unit, typename Point>
		using utf32_le = swap_endian<utf32<Unit, Point>>;
		template <typename Unit, typename Point>
		using utf32_be = utf32<Unit, Point>;
		#endif
	}
}

#endif