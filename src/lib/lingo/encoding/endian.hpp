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
			static LINGO_CONSTEXPR14 encode_result encode_point(
				typename Encoding::point_type point,
				typename Encoding::unit_type* buffer,
				typename Encoding::size_type buffer_size) noexcept
			{
				// Encode point into intermediate buffer
				typename Encoding::unit_type intermediate_buffer[Encoding::max_units] = {};
				const auto intermediate_result = Encoding::encode_point(point, intermediate_buffer, Encoding::max_units);

				// Check for errors
				if (intermediate_result.error != error::error_code::success)
				{
					return intermediate_result;
				}
				if (buffer_size < intermediate_result.size)
				{
					return { intermediate_result.size, error::error_code::buffer_too_small };
				}

				// Copy and swap data from intermediate buffer to output buffer
				for (typename Encoding::size_type i = 0; i < intermediate_result.size; ++i)
				{
					buffer[i] = platform::swap_endian(intermediate_buffer[i]);
				}

				// Return result
				return { intermediate_result.size, error::error_code::success };
			}

			static LINGO_CONSTEXPR14 decode_result<typename Encoding::point_type> decode_point(
				const typename Encoding::unit_type* buffer, 
				typename Encoding::size_type buffer_size) noexcept
			{
				// Copy and swap data from output buffer to intermediate buffer
				typename Encoding::unit_type intermediate_buffer[Encoding::max_units] = {};
				const size_t size = std::min(buffer_size, Encoding::max_units);
				for (typename Encoding::size_type i = 0; i < size; ++i)
				{
					intermediate_buffer[i] = platform::swap_endian(buffer[i]);
				}

				// Decode point into intermediate buffer
				return Encoding::decode_point(intermediate_buffer, size);
			}
		};
	}
}

#endif