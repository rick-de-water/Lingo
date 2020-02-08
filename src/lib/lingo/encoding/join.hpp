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

			using encode_result_type = encode_result<unit_type, point_type>;
			using decode_result_type = decode_result<unit_type, point_type>;
			using encode_source_type = typename encode_result_type::source_type;
			using decode_source_type = typename decode_result_type::source_type;
			using encode_destination_type = typename encode_result_type::destination_type;
			using decode_destination_type = typename decode_result_type::destination_type;

			struct encode_state_type
			{
				typename first_encoding::encode_state_type first_state;
				typename base_encoding::encode_state_type base_state;
			};

			struct decode_state_type
			{
				typename first_encoding::decode_state_type first_state;
				typename base_encoding::decode_state_type base_state;
			};

			static LINGO_CONSTEXPR11 const size_type max_units = first_encoding::max_units * base_encoding::max_units;

			static_assert(std::is_same<typename first_encoding::point_type, typename base_encoding::unit_type>::value, "The point_type of an encoding must match the unit_type of the next encoding");

			static LINGO_CONSTEXPR14 encode_result_type encode_one(encode_source_type source, encode_destination_type destination, encode_state_type& state, bool final) noexcept
			{
				// Encode to base encoding
				typename base_encoding::unit_type base_destination_buffer[base_encoding::max_units];
				utility::span<typename base_encoding::unit_type> base_destination(base_destination_buffer);
				const auto base_result = base_encoding::encode_one(source, base_destination, state.base_state, final);
				if (base_result.error != lingo::error::error_code::success)
				{
					return { source, destination, base_result.error };
				}

				// Encode base units to first encoding
				utility::span<const typename first_encoding::point_type> first_source = base_destination.diff(base_result.destination);
				auto first_destination = destination;
				while (first_source.size() > 0)
				{
					const auto first_result = first_encoding::encode_one(first_source, first_destination, state.first_state, final && base_result.source.size() == 0);
					if (first_result.error != lingo::error::error_code::success)
					{
						return { source, destination, first_result.error };
					}

					first_source = first_result.source;
					first_destination = first_result.destination;
				}

				return { base_result.source, first_destination, lingo::error::error_code::success };
			}

			static LINGO_CONSTEXPR14 decode_result_type decode_one(decode_source_type source, decode_destination_type destination, decode_state_type& state, bool final) noexcept
			{
				auto first_source = source;
				typename base_encoding::unit_type first_destination_buffer[base_encoding::max_units];
				utility::span<typename base_encoding::unit_type> first_destination(first_destination_buffer);
				for (size_type i = 0; i < base_encoding::max_units; ++i)
				{
					// Decode from first encoding
					const auto first_result = first_encoding::decode_one(first_source, first_destination, state.first_state, final);
					if (first_result.error != lingo::error::error_code::success)
					{
						return { source, destination, first_result.error };
					}

					first_source = first_result.source;
					first_destination = first_result.destination;

					// Try to decode to base encoding
					const auto base_result = base_encoding::decode_one(
						utility::span<typename base_encoding::unit_type>(first_destination_buffer).diff(first_destination),
						destination, state.base_state, final && first_source.size() == 0);
					if (base_result.error != lingo::error::error_code::success)
					{
						if (base_result.error == lingo::error::error_code::source_buffer_too_small)
						{
							continue;
						}
						else
						{
							return { source, destination, base_result.error };
						}
					}
					
					// Return result
					return { first_source, base_result.destination, lingo::error::error_code::success };
				}
				// Should never be able to reach this.
				// Either the code point is decoded successfully or an error occurs within base_encoding::max_units cycles
				assert(false);
			}
		};

		template <typename LastEncoding>
		struct join<LastEncoding> : LastEncoding
		{
		};
	}
}

#endif