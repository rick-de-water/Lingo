#ifndef H_LINGO_ENCODING_BASE
#define H_LINGO_ENCODING_BASE

#include <lingo/platform/constexpr.hpp>

#include <lingo/encoding/endian.hpp>
#include <lingo/encoding/result.hpp>
#include <lingo/encoding/internal/bit_converter.hpp>

#include <array>
#include <climits>

namespace lingo
{
	namespace encoding
	{
		template <typename Unit, typename Point, std::size_t BitsPerUnit, const Unit* TableToBase, const Point* TableFromBase, Unit Padding>
		struct base
		{
			public:
			using unit_type = Unit;
			using point_type = Point;

			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;

			static LINGO_CONSTEXPR11 const unit_type* table_to_base = TableToBase;
			static LINGO_CONSTEXPR11 const point_type* table_from_base = TableFromBase;
			static LINGO_CONSTEXPR11 const unit_type padding = Padding;

			static LINGO_CONSTEXPR11 const size_type max_units = 4;
			static LINGO_CONSTEXPR11 const size_type min_unit_bits = BitsPerUnit;
			static LINGO_CONSTEXPR11 const size_type min_point_bits = min_unit_bits;

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
			static LINGO_CONSTEXPR14 encode_result_type encode_one(encode_source_type source, encode_destination_type destination, encode_state_type&) noexcept
			{
				if (source.size() < 1)
				{
					return { source, destination, error::error_code::source_buffer_too_small };
				}

				if (destination.size() < 1)
				{
					return { source, destination, error::error_code::destination_buffer_too_small };
				}

				const point_bits_type point_bits = bit_converter_type::to_point_bits(source[0]);
				const unit_bits_type unit_bits = static_cast<unit_bits_type>(point_bits);
				destination[0] = bit_converter_type::from_unit_bits(unit_bits);

				return { source.subspan(1), destination.subspan(1), error::error_code::success };
			}

			static LINGO_CONSTEXPR14 decode_result_type decode_one(decode_source_type source, decode_destination_type destination, decode_state_type&) noexcept
			{
				if (source.size() < 1)
				{
					return { source, destination, error::error_code::source_buffer_too_small };
				}

				if (destination.size() < 1)
				{
					return { source, destination, error::error_code::destination_buffer_too_small };
				}

				const unit_bits_type unit_bits = bit_converter_type::to_unit_bits(source[0]);
				const point_bits_type point_bits = static_cast<point_bits_type>(unit_bits);
				destination[0] = bit_converter_type::from_point_bits(point_bits);

				return { source.subspan(1), destination.subspan(1), error::error_code::success };
			}
		};

		template <typename Unit, typename Point, std::size_t UnitBits, const Unit* TableToBase, const Point* TableFromBase, Unit Padding>
		LINGO_CONSTEXPR11 Unit base<Unit, Point, UnitBits, TableToBase, TableFromBase, Padding>::padding;

		namespace internal
		{
			template <typename Unit>
			struct base64_to_base_table
			{
				static LINGO_CONSTEXPR11 const Unit value[] =
				{
					0x41, 0x42, 0x43, 0x44,
					0x45, 0x46, 0x47, 0x48,
					0x49, 0x4A, 0x4B, 0x4C,
					0x4D, 0x4E, 0x4F, 0x50,
					0x51, 0x52, 0x53, 0x54,
					0x55, 0x56, 0x57, 0x58,
					0x59, 0x5A, 0x61, 0x62,
					0x63, 0x64, 0x65, 0x66,
					0x67, 0x68, 0x69, 0x6A,
					0x6B, 0x6C, 0x6D, 0x6E,
					0x6F, 0x70, 0x71, 0x72,
					0x73, 0x74, 0x75, 0x76,
					0x77, 0x78, 0x79, 0x7A,
					0x30, 0x31, 0x32, 0x33,
					0x34, 0x35, 0x36, 0x37,
					0x38, 0x39, 0x2B, 0x2F
				};
			};

			template <typename Unit>
			LINGO_CONSTEXPR11 Unit base64_to_base_table<Unit>::value[];

			template <typename Point>
			struct base64_from_base_table
			{
				static LINGO_CONSTEXPR11 const Point value[] =
				{
					0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x3F,
					0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
					0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00,
					0x00, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
					0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00
				};
			};

			template <typename Unit>
			LINGO_CONSTEXPR11 Unit base64_from_base_table<Unit>::value[];
		}

		template <typename Unit, typename Point, Unit Padding = 0x3D>
		using base64 = base<Unit, Point, 6, internal::base64_to_base_table<Unit>::value, internal::base64_from_base_table<Point>::value, Padding>;
	}
}

#endif