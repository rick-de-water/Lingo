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

			static LINGO_CONSTEXPR14 encode_result_type encode_one(encode_source_type source, encode_destination_type destination, encode_state_type& state) noexcept
			{
				typename base_encoding::unit_type base_destination_buffer[base_encoding::max_units];
				utility::span<typename base_encoding::unit_type> base_destination(base_destination_buffer);
				const auto base_result = base_encoding::encode_one(source, base_destination, state.base_state);
				if (base_result.error != lingo::error::error_code::success)
				{
					return base_result;
				}

				const auto first_source = base_destination.diff(base_result.destination);
				auto first_destination = destination;
				for (size_type i = 0; i < first_source.size(); ++i)
				{
					utility::span<const typename first_encoding::point_type> first_subspan = first_source.subspan(i, 1);
					const auto first_result = first_encoding::encode_one(first_subspan, first_destination, state.first_state);
					if (first_result.error != lingo::error::error_code::success)
					{
						return { source, destination, first_result.error };
					}

					first_destination = first_result.destination;
				}

				return { base_result.source, first_destination, lingo::error::error_code::success };
			}

			static LINGO_CONSTEXPR14 decode_result_type decode_one(encode_source_type, encode_destination_type, decode_state_type&) noexcept
			{
				return {};
			}
		};

		template <typename LastEncoding>
		struct join<LastEncoding> : LastEncoding
		{
		};
	}
}

#endif