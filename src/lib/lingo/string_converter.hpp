#ifndef H_LINGO_STRING_CONVERTER
#define H_LINGO_STRING_CONVERTER

#include <lingo/conversion_result.hpp>
#include <lingo/encoding/utf8.hpp>
#include <lingo/error/strict.hpp>
#include <lingo/page/point_mapper.hpp>
#include <lingo/page/unicode.hpp>
#include <lingo/platform/warnings.hpp>
#include <lingo/utility/span.hpp>

#include <cassert>
#include <cstddef>

namespace lingo
{
	template <typename Encoding, typename Page, typename Allocator>
	class basic_string;

	template <typename Encoding, typename Page>
	class basic_string_view;

	template <
		typename SourceEncoding, typename SourcePage,
		typename DestinationEncoding, typename DestinationPage>
	class string_converter
	{
		public:
		using source_encoding_type = SourceEncoding;
		using source_page_type = SourcePage;
		using source_unit_type = typename source_encoding_type::unit_type;
		using source_point_type = typename source_encoding_type::point_type;
		using source_encoding_state = typename source_encoding_type::encoding_state;
		using source_decoding_state = typename source_encoding_type::decoding_state;

		using destination_encoding_type = DestinationEncoding;
		using destination_page_type = DestinationPage;
		using destination_unit_type = typename destination_encoding_type::unit_type;
		using destination_point_type = typename destination_encoding_type::point_type;
		using destination_encoding_state = typename destination_encoding_type::encoding_state;
		using destination_decoding_state = typename destination_encoding_type::decoding_state;

		using point_mapper = page::point_mapper<source_page_type, destination_page_type>;

		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;

		static_assert(std::is_same<typename source_page_type::point_type, typename source_encoding_type::point_type>::value, "source_page_type::point_type must be the same type as source_encoding_type::point_type");
		static_assert(std::is_same<typename destination_page_type::point_type, typename destination_encoding_type::point_type>::value, "destination_page_type::point_type must be the same type as destination_encoding_type::point_type");

		LINGO_CONSTEXPR14 conversion_result convert(utility::span<const source_unit_type> source, utility::span<destination_unit_type> destination)
		{
			utility::span<const source_unit_type> read_buffer = source;
			utility::span<destination_unit_type> write_buffer = destination;

			source_decoding_state read_state;
			destination_encoding_state write_state;
			
			while (read_buffer.size() > 0 && write_buffer.size() > 0)
			{
				source_point_type source_point_buffer[source_encoding_type::max_points];

				// Try to decode a point from the source
				const encoding::decode_result decode_result = source_encoding_type::decode_point(read_buffer, source_point_buffer, read_state);
				if (decode_result.error != error::error_code::success)
				{
					if (!handle_error(decode_result, read_buffer, source_point_buffer))
					{
						break;
					}
				}

				// Try to map the source poinst to a destination points
				source_point_type destination_point_buffer[source_encoding_type::max_points];
				for (size_type i = 0; i < decode_result.source_read; ++i)
				{
					const auto map_result = point_mapper::map(source_point_buffer[i]);
					if (map_result.error != error::error_code::success)
					{
						break;
					}

					destination_point_buffer[i] = map_result.point;
				}

				// Try to encode the point into the destination buffer
				const encoding::encode_result encode_result = destination_encoding_type::encode_point(destination_point_buffer, write_buffer, write_state);
				if (encode_result.error != error::error_code::success)
				{
					// A destination buffer that is too small is not considered an error.
					// Return from this function and allow the callee to provide more buffer space
					if (encode_result.error == error::error_code::destination_buffer_too_small)
					{
						break;
					}
					else if (!handle_error(encode_result, destination_point_buffer, write_buffer))
					{
						break;
					}
				}

				read_buffer = read_buffer.subspan(decode_result.source_read);
				write_buffer = write_buffer.subspan(encode_result.destination_written);
			}

			return { source.size() - read_buffer.size(), destination.size() - write_buffer.size() };
		}

		LINGO_CONSTEXPR14 conversion_result convert(
			basic_string_view<source_encoding_type, source_page_type> source,
			destination_unit_type* destination_buffer, size_type destination_buffer_size)
		{
			return convert(source.data(), source.size(), destination_buffer, destination_buffer_size);
		}

		template <typename Allocator>
		basic_string<destination_encoding_type, destination_page_type, Allocator> convert(basic_string_view<source_encoding_type, source_page_type> source, const Allocator& allocator = Allocator())
		{
			basic_string<destination_encoding_type, destination_page_type, Allocator> string(allocator);

			string.resize(source.size());

			size_type total_units_read = 0;
			size_type total_units_written = 0;

			while (total_units_read < source.size())
			{
				auto result = convert(
					utility::span<const source_unit_type>(source.data() + total_units_read, source.size() - total_units_read),
					utility::span<destination_unit_type>(string.data() + total_units_written, string.size() - total_units_written));

				total_units_read += result.source_read;
				total_units_written += result.destination_written;

				if (total_units_read < source.size())
				{
					string.resize(string.size() * 2);
				}
			}

			string.resize(total_units_written);
			return string;
		}

		private:
		LINGO_WARNINGS_PUSH_AND_DISABLE_MSVC(4702)
		bool handle_error(encoding::decode_result result,
			utility::span<const source_unit_type> source,
			utility::span<source_point_type> destination)
		{
			// No error
			if (result.error == error::error_code::success)
			{
				return true;
			}

			// Attempt to handle the error
			result = error::strict<source_encoding_type, source_page_type>::handle(result, source, destination);
			return result.error == error::error_code::success;
		}

		bool handle_error(encoding::encode_result result,
			utility::span<const destination_point_type> source,
			utility::span<destination_unit_type> destination)
		{
			// No error
			if (result.error == error::error_code::success)
			{
				return true;
			}

			// Attempt to handle the error
			result = error::strict<destination_encoding_type, destination_page_type>::handle(result, source, destination);
			return result.error == error::error_code::success;
		}
		LINGO_WARNINGS_POP_MSVC
	};
}

#endif