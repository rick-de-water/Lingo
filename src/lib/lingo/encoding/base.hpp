#ifndef H_LINGO_ENCODING_BASE
#define H_LINGO_ENCODING_BASE

#include <lingo/platform/constexpr.hpp>

#include <lingo/encoding/endian.hpp>
#include <lingo/encoding/result.hpp>
#include <lingo/encoding/internal/bit_converter.hpp>

#include <algorithm>
#include <array>
#include <climits>

namespace lingo
{
	namespace encoding
	{
		template <typename Unit, typename Point, std::size_t BitsPerUnit, const Unit* TableToBase, const Unit* TableFromBase, Unit Padding>
		struct base
		{
			public:
			using unit_type = Unit;
			using point_type = Point;

			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;

			static LINGO_CONSTEXPR11 const unit_type* table_to_base = TableToBase;
			static LINGO_CONSTEXPR11 const unit_type* table_from_base = TableFromBase;
			static LINGO_CONSTEXPR11 const unit_type padding = Padding;

			static LINGO_CONSTEXPR11 const size_type max_units = (sizeof(point_type) * CHAR_BIT + BitsPerUnit - 1) / BitsPerUnit;
			static LINGO_CONSTEXPR11 const size_type min_unit_bits = BitsPerUnit;
			static LINGO_CONSTEXPR11 const size_type min_point_bits = min_unit_bits;

			using encode_result_type = encode_result<unit_type, point_type>;
			using decode_result_type = decode_result<unit_type, point_type>;
			using encode_source_type = typename encode_result_type::source_type;
			using decode_source_type = typename decode_result_type::source_type;
			using encode_destination_type = typename encode_result_type::destination_type;
			using decode_destination_type = typename decode_result_type::destination_type;

			private:
			using bit_converter_type = internal::bit_converter<unit_type, min_unit_bits, point_type, min_point_bits>;
			using unit_bits_type = typename bit_converter_type::unit_bits_type;
			using point_bits_type = typename bit_converter_type::point_bits_type;

			static LINGO_CONSTEXPR11 const size_type bits_per_unit = BitsPerUnit;
			static LINGO_CONSTEXPR11 const size_type bits_per_point = sizeof(point_bits_type) * CHAR_BIT;
			static LINGO_CONSTEXPR11 const unit_bits_type unit_bit_mask = static_cast<unit_bits_type>(-1);
			static LINGO_CONSTEXPR11 const point_bits_type point_bit_mask = static_cast<point_bits_type>(-1);

			static_assert(bits_per_unit <= sizeof(unit_type) * CHAR_BIT, "Unit is smaller than BitsPerUnit");

			public:
			struct encode_state_type
			{
				unit_bits_type unit_bits = 0;
				size_type unit_bit_count = 0;
				size_type total_bit_count = 0;
			};

			struct decode_state_type
			{
				unit_bits_type unit_bits = 0;
				size_type unit_bit_count = 0;
				size_type total_bit_count = 0;
			};

			public:
			static LINGO_CONSTEXPR14 encode_result_type encode_one(encode_source_type source, encode_destination_type destination, encode_state_type& state, bool final) noexcept
			{
				if (source.size() < 1)
				{
					return { source, destination, error::error_code::source_buffer_too_small };
				}

				// Get the bits of the next point
				point_bits_type point_bits = bit_converter_type::to_point_bits(source[0]);
				size_type point_bit_count = bits_per_point;

				// Start generating points
				size_type unit_count = 0;
				unit_bits_type unit_bits = state.unit_bits;
				size_type unit_bit_count = state.unit_bit_count;
				size_type total_bit_count = state.total_bit_count;
				encode_destination_type unit_buffer = destination;

				// While there are still point bits left
				while (point_bit_count > 0)
				{
					// Take some bits
					const size_type bit_count = std::min(bits_per_unit - unit_bit_count, point_bit_count);
					const size_type bit_shift = (bits_per_point - bit_count);
					const point_bits_type bit_mask = point_bit_mask << bit_shift;
					point_bits_type bits = (point_bits & bit_mask) >> bit_shift;

					// Add them to the existing bits
					unit_bits |= bits << (bits_per_unit - (unit_bit_count + bit_count));

					// Update bit counters
					unit_bit_count += bit_count;
					point_bits <<= bit_count;
					point_bit_count -= bit_count;
					total_bit_count += bit_count;
					assert(unit_bit_count <= bits_per_unit);

					// Add unit if it is complete
					if (unit_bit_count == bits_per_unit)
					{
						if (unit_buffer.size() < 1)
						{
							return { source, destination, error::error_code::destination_buffer_too_small };
						}

						unit_buffer[0] = table_to_base[unit_bits];
						unit_buffer = unit_buffer.subspan(1);

						++unit_count;
						unit_bits = 0;
						unit_bit_count = 0;
					}
				}

				// Pad if final
				if (final && source.size() == 1)
				{
					// Add last few bits
					if (unit_bit_count != 0)
					{
						if (unit_buffer.size() < 1)
						{
							return { source, destination, error::error_code::destination_buffer_too_small };
						}

						unit_buffer[0] = table_to_base[unit_bits];
						unit_buffer = unit_buffer.subspan(1);
						total_bit_count += bits_per_unit - unit_bit_count;
						++unit_count;
					}

					// Pad until divisible by bits_per_point
					while (total_bit_count % bits_per_point != 0)
					{
						if (unit_buffer.size() < 1)
						{
							return { source, destination, error::error_code::destination_buffer_too_small };
						}

						unit_buffer[0] = padding;
						unit_buffer = unit_buffer.subspan(1);
						total_bit_count += bits_per_unit;
						++unit_count;
					}
				}
				// Store the leftover bits in the state
				else
				{
					state.unit_bits = unit_bits;
					state.unit_bit_count = unit_bit_count;
					state.total_bit_count = total_bit_count;
				}

				return { source.subspan(1), destination.subspan(unit_count), error::error_code::success };
			}

			static LINGO_CONSTEXPR14 decode_result_type decode_one(decode_source_type source, decode_destination_type destination, decode_state_type& state, bool final) noexcept
			{
				if (destination.size() < 1)
				{
					return { source, destination, error::error_code::destination_buffer_too_small };
				}

				// Initialize counters
				size_type point_bit_count = state.unit_bit_count;
				point_bits_type point_bits = static_cast<point_bits_type>(state.unit_bits) << (bits_per_point - point_bit_count);

				size_type unit_count = 0;
				unit_bits_type unit_bits = 0;
				size_type unit_bit_count = 0;
				size_type total_bit_count = state.total_bit_count;
				decode_source_type unit_buffer = source;

				while (point_bit_count < bits_per_point)
				{
					// Pop the next unit
					if (unit_bit_count == 0)
					{
						if (unit_buffer.size() < 1)
						{
							return { source, destination, error::error_code::source_buffer_too_small };
						}

						unit_type last_unit = unit_buffer[0];
						assert(last_unit != padding);
						unit_bits = bit_converter_type::to_unit_bits(table_from_base[last_unit]);
						unit_bit_count = bits_per_unit;
						unit_buffer = unit_buffer.subspan(1);
						++unit_count;
						total_bit_count += unit_bit_count;
					}

					// Add the bits to the point type
					const size_type bit_count = std::min(unit_bit_count, bits_per_point - point_bit_count);
					const point_bits_type bits = unit_bits >> (bits_per_unit - bit_count);
					const size_type bit_shift = bits_per_point - (point_bit_count + bit_count);
					point_bits |= bits << bit_shift;

					unit_bit_count -= bit_count;
					unit_bits &= unit_bit_mask >> (bit_count + (sizeof(unit_type) * CHAR_BIT - bits_per_unit));
					point_bit_count += bit_count;
				}

				// Check for padding
				if (unit_bit_count > 0)
				{
					if (unit_buffer.size() < 1)
					{
						return { source, destination, error::error_code::source_buffer_too_small };
					}

					// Padding found
					if (unit_buffer[0] == padding)
					{
						// Loop until bit count aligns with point size
						while (total_bit_count % bits_per_point != 0)
						{
							// Check if we have more units
							if (unit_buffer.size() < 1)
							{
								return { source, destination, error::error_code::source_buffer_too_small };
							}

							// Check if it is padded
							if (unit_buffer[0] != padding)
							{
								return { source, destination, error::error_code::invalid_unit };
							}

							unit_buffer = unit_buffer.subspan(1);
							++unit_count;
							total_bit_count += bits_per_unit;
						}

						// We've reached the end, make sure that there is no more data
						if (unit_buffer.size() > 0 || !final)
						{
							return { source, destination, error::error_code::invalid_unit };
						}
					}
				}


				// Store the leftover bits in the state
				state.unit_bits = unit_bits;
				state.unit_bit_count = unit_bit_count;
				state.total_bit_count = total_bit_count;

				// Copy point to destination
				destination[0] = bit_converter_type::from_point_bits(point_bits);

				// Return result
				return { source.subspan(unit_count), destination.subspan(1), error::error_code::success };
			}
		};

		template <typename Unit, typename Point, std::size_t UnitBits, const Unit* TableToBase, const Unit* TableFromBase, Unit Padding>
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

			template <typename Unit>
			struct base64_from_base_table
			{
				static LINGO_CONSTEXPR11 const Unit value[] =
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
		using base64 = base<Unit, Point, 6, internal::base64_to_base_table<Unit>::value, internal::base64_from_base_table<Unit>::value, Padding>;
	}
}

#endif