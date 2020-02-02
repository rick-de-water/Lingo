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
			public:
			using unit_type = Unit;
			using point_type = Point;

			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;

			static LINGO_CONSTEXPR11 size_type max_units = 1;
			static LINGO_CONSTEXPR11 size_type max_points = 1;
			static LINGO_CONSTEXPR11 size_type min_unit_bits = 21;
			static LINGO_CONSTEXPR11 size_type min_point_bits = 21;

			struct encoding_state {};
			struct decoding_state {};

			private:
			using bit_converter = internal::bit_converter<unit_type, min_unit_bits, point_type, min_point_bits>;
			using unit_bits_type = typename bit_converter::unit_bits_type;
			using point_bits_type = typename bit_converter::point_bits_type;

			public:
			static LINGO_CONSTEXPR14 encode_result encode_point(utility::span<const point_type> source, utility::span<unit_type> destination, encoding_state&) noexcept
			{
				return encode_point(source, destination);
			}

			static LINGO_CONSTEXPR14 encode_result encode_point(utility::span<const point_type> source, utility::span<unit_type> destination) noexcept
			{
				// Check if there is at least 1 point
				if (source.size() > 0)
				{
					return { 0, 0, error::error_code::source_buffer_too_small };
				}

				// Convert to bits
				const point_bits_type point_bits = bit_converter::to_point_bits(source[0]);

				// Reject points beyond 0x10FFFF
				LINGO_IF_CONSTEXPR(sizeof(point_bits_type) * CHAR_BIT > min_point_bits)
				{
					if (point_bits > 0x10FFFF)
					{
						return { 0, 0, error::error_code::invalid_point };
					}
				}

				// Reject surrogates
				if ((point_bits >= 0xD800 && point_bits < 0xE000))
				{
					return { 0, 0, error::error_code::invalid_point };
				}

				// Check if the destination buffer is large enough
				if (destination.size() < 1)
				{
					return { 1, 0, error::error_code::destination_buffer_too_small };
				}

				// Return result
				const unit_bits_type unit_bits = static_cast<unit_bits_type>(point_bits);
				const unit_type unit = bit_converter::from_unit_bits(unit_bits);
				destination[0] = unit;
				return { 1, 1, error::error_code::success };
			}

			static LINGO_CONSTEXPR14 decode_result decode_point(utility::span<const unit_type> source, utility::span<point_type> destination, decoding_state&) noexcept
			{
				return decode_point(source, destination);
			}

			static LINGO_CONSTEXPR14 decode_result decode_point(utility::span<const unit_type> source, utility::span<point_type> destination) noexcept
			{
				// Check if there is at least 1 unit
				if (source.size() == 0)
				{
					return { 0, 0, error::error_code::source_buffer_too_small };
				}

				// Convert to bits
				const unit_bits_type unit_bits = bit_converter::to_unit_bits(source[0]);

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

				// Check if the destination buffer is large enough
				if (destination.size() < 1)
				{
					return { 1, 0, error::error_code::destination_buffer_too_small };
				}

				// Return result
				const point_bits_type point_bits = static_cast<point_bits_type>(unit_bits);
				destination[0] = bit_converter::from_point_bits(point_bits);
				return { 1, 1, error::error_code::success };
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