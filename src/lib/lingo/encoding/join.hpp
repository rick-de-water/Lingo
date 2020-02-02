#ifndef H_LINGO_ENCODING_JOIN
#define H_LINGO_ENCODING_JOIN

#include <lingo/platform/constexpr.hpp>

#include <lingo/encoding/result.hpp>

#include <climits>
#include <type_traits>

namespace lingo
{
	namespace encoding
	{
		template <typename FirstEncoding, typename... OtherEncodings>
		struct join : join<OtherEncodings...>
		{
			using first_encoding = FirstEncoding;
			using base_encoding = join<OtherEncodings...>;

			using unit_type = typename first_encoding::unit_type;
			using point_type = typename base_encoding::point_type;

			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;

			struct encoding_state
			{
				typename first_encoding::encoding_state first_state;
				typename base_encoding::encoding_state base_state;
			};

			struct decoding_state
			{
				typename first_encoding::decoding_state first_state;
				typename base_encoding::decoding_state base_state;
			};

			static LINGO_CONSTEXPR11 const size_type max_units = first_encoding::max_units * base_encoding::max_units;

			static_assert(std::is_same<typename first_encoding::point_type, typename base_encoding::unit_type>::value, "The point_type of an encoding must match the unit_type of the next encoding");

			static LINGO_CONSTEXPR14 encode_result encode_point(utility::span<const point_type> source, utility::span<unit_type> destination, encoding_state& state) noexcept
			{
				typename base_encoding::unit_type base_destination_buffer[base_encoding::max_units];
				utility::span<typename base_encoding::unit_type> base_destination(base_destination_buffer);
				const encode_result base_result = base_encoding::encode_point(source, base_destination, state.base_state);
				if (base_result.error != lingo::error::error_code::success)
				{
					return base_result;
				}

				utility::span<const typename first_encoding::point_type> first_source(base_destination.subspan(0, base_result.destination_written));
				size_type first_buffer_offset = 0;
				for (size_type i = 0; i < base_destination.size(); ++i)
				{
					utility::span<const typename first_encoding::point_type> first_subspan = first_source.subspan(i, 1);
					const encode_result first_result = first_encoding::encode_point(first_subspan, destination.subspan(first_buffer_offset), state.first_state);
					if (first_result.error != lingo::error::error_code::success)
					{
						return first_result;
					}

					first_buffer_offset += first_result.destination_written;
				}

				return { first_source.size(), first_buffer_offset, lingo::error::error_code::success };
			}

			static LINGO_CONSTEXPR14 decode_result decode_point(utility::span<const point_type> source, utility::span<unit_type> destination, decoding_state& state) noexcept
			{
				return {};
			}
		};

		template <typename LastEncoding>
		struct join<LastEncoding> : LastEncoding
		{
			//using encoding_state = typename LastEncoding::encoding_state;
			//using decoding_state = typename LastEncoding::decoding_state;
		};
	}
}

#endif