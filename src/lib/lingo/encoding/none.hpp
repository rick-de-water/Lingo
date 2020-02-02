#ifndef H_LINGO_ENCODING_NONE
#define H_LINGO_ENCODING_NONE

#include <lingo/platform/constexpr.hpp>

#include <lingo/encoding/endian.hpp>
#include <lingo/encoding/result.hpp>
#include <lingo/encoding/internal/bit_converter.hpp>

#include <climits>

namespace lingo
{
	namespace encoding
	{
		template <typename Unit, typename Point>
		struct none
		{
			public:
			using unit_type = Unit;
			using point_type = Point;

			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;

			static LINGO_CONSTEXPR11 size_type max_units = 1;
			static LINGO_CONSTEXPR11 size_type max_points = 1;
			static LINGO_CONSTEXPR11 size_type min_point_bits = sizeof(point_type) * CHAR_BIT;
			static LINGO_CONSTEXPR11 size_type min_unit_bits = min_point_bits;

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
				if (source.size() < 1)
				{
					return { 0, 0, error::error_code::source_buffer_too_small };
				}

				if (destination.size() < 1)
				{
					return { 1, 0, error::error_code::destination_buffer_too_small };
				}

				const point_bits_type point_bits = bit_converter::to_point_bits(source[0]);
				const unit_bits_type unit_bits = static_cast<unit_bits_type>(point_bits);
				destination[0] = bit_converter::from_unit_bits(unit_bits);

				return { 1, 1, error::error_code::success };
			}

			static LINGO_CONSTEXPR14 encode_result encode_point(utility::span<const point_type> source, utility::span<unit_type> destination, decoding_state&) noexcept
			{
				return decode_point(source, destination);
			}

			static LINGO_CONSTEXPR14 decode_result decode_point(utility::span<const unit_type> source, utility::span<point_type> destination) noexcept
			{
				if (source.size() < 1)
				{
					return { 0, 0, error::error_code::source_buffer_too_small };
				}

				if (destination.size() < 1)
				{
					return { 1, 0, error::error_code::destination_buffer_too_small };
				}

				const unit_bits_type unit_bits = bit_converter::to_unit_bits(source[0]);
				const point_bits_type point_bits = static_cast<point_bits_type>(unit_bits);
				destination[0] = bit_converter::from_point_bits(point_bits);

				return { 1, 1, error::error_code::success };
			}
		};

		#if LINGO_ARCHITECTURE_ENDIANNESS == LINGO_ARCHITECTURE_LITTLE_ENDIAN
		template <typename Unit, typename Point>
		using none_le = none<Unit, Point>;
		template <typename Unit, typename Point>
		using none_be = swap_endian<none<Unit, Point>>;
		#else
		template <typename Unit, typename Point>
		using none_le = swap_endian<none<Unit, Point>>;
		template <typename Unit, typename Point>
		using none_be = none<Unit, Point>;
		#endif
	}
}

#endif