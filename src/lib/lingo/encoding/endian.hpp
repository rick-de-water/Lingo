#ifndef H_LINGO_ENCODING_ENDIAN
#define H_LINGO_ENCODING_ENDIAN

#include <lingo/platform/constexpr.hpp>
#include <lingo/platform/endian.hpp>

#include <lingo/encoding/result.hpp>

#include <algorithm>

namespace lingo
{
	namespace encoding
	{
		template <typename Encoding>
		struct swap_endian : public Encoding
		{
			static LINGO_CONSTEXPR14 typename Encoding::encode_result_type encode_one(
				utility::span<const typename Encoding::point_type> source,
				utility::span<typename Encoding::unit_type> destination,
				typename Encoding::encode_state_type& state) noexcept
			{
				// Encode point into intermediate buffer
				typename Encoding::unit_type intermediate_buffer[Encoding::max_units] = {};
				utility::span<typename Encoding::unit_type> intermediate_span(intermediate_buffer);
				const auto intermediate_result = Encoding::encode_one(source, intermediate_span, state);

				// Check for errors
				if (intermediate_result.error != error::error_code::success)
				{
					return intermediate_result;
				}
				auto written = intermediate_span.diff(intermediate_result.destination);
				if (destination.size() < written.size())
				{
					return { source, destination, error::error_code::destination_buffer_too_small };
				}

				// Copy and swap data from intermediate buffer to destination buffer
				for (typename Encoding::size_type i = 0; i < written.size(); ++i)
				{
					destination[i] = platform::swap_endian(intermediate_buffer[i]);
				}

				// Return result
				return { intermediate_result.source, intermediate_result.destination, error::error_code::success };
			}

			static LINGO_CONSTEXPR14 typename Encoding::decode_result_type decode_one(
				utility::span<const typename Encoding::unit_type> source,
				utility::span<typename Encoding::point_type> destination,
				typename Encoding::decode_state_type& state) noexcept
			{
				// Copy and swap data from destination buffer to intermediate buffer
				typename Encoding::unit_type intermediate_buffer[Encoding::max_units] = {};
				const size_t size = std::min(source.size(), Encoding::max_units);
				for (typename Encoding::size_type i = 0; i < size; ++i)
				{
					intermediate_buffer[i] = platform::swap_endian(source[i]);
				}

				// Decode point into intermediate buffer
				return Encoding::decode_one(intermediate_buffer, destination, state);
			}
		};
	}
}

#endif