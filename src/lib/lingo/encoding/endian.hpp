#ifndef H_LINGO_ENCODING_ENDIAN
#define H_LINGO_ENCODING_ENDIAN

#include <lingo/platform/constexpr.hpp>
#include <lingo/platform/endian.hpp>

#include <lingo/encoding/result.hpp>

#include <lingo/utility/span.hpp>

#include <algorithm>

namespace lingo
{
	namespace encoding
	{
		template <typename Encoding>
		struct swap_endian : public Encoding
		{
			static LINGO_CONSTEXPR14 encode_result encode_point(
				utility::span<const typename Encoding::point_type> source,
				utility::span<typename Encoding::unit_type> destination,
				typename Encoding::encoding_state& state) noexcept
			{
				// Encode point into intermediate buffer
				typename Encoding::unit_type intermediate_buffer[Encoding::max_units] = {};
				const auto intermediate_result = Encoding::encode_point(source, intermediate_buffer, state);

				// Check for errors
				if (intermediate_result.error != error::error_code::success)
				{
					return intermediate_result;
				}
				if (destination.size() < intermediate_result.destination_written)
				{
					return { intermediate_result.destination_written, 0, error::error_code::destination_buffer_too_small };
				}

				// Copy and swap data from intermediate buffer to destination buffer
				for (typename Encoding::size_type i = 0; i < intermediate_result.destination_written; ++i)
				{
					destination[i] = platform::swap_endian(intermediate_buffer[i]);
				}

				// Return result
				return { 1, intermediate_result.destination_written, error::error_code::success };
			}

			static LINGO_CONSTEXPR14 decode_result decode_point(
				utility::span<const typename Encoding::unit_type> source,
				utility::span<typename Encoding::point_type> destination,
				typename Encoding::decoding_state& state) noexcept
			{
				// Copy and swap data from destination buffer to intermediate buffer
				typename Encoding::unit_type intermediate_buffer[Encoding::max_units] = {};
				const size_t size = std::min(source.size(), Encoding::max_units);
				for (typename Encoding::size_type i = 0; i < size; ++i)
				{
					intermediate_buffer[i] = platform::swap_endian(source[i]);
				}

				// Decode point into intermediate buffer
				return Encoding::decode_point(intermediate_buffer, destination, state);
			}
		};
	}
}

#endif