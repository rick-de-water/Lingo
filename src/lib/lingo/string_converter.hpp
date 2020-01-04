#ifndef H_LINGO_STRING_CONVERTER
#define H_LINGO_STRING_CONVERTER

#include <lingo/conversion_result.hpp>
#include <lingo/encoding/utf8.hpp>
#include <lingo/error/strict.hpp>
#include <lingo/page/point_mapper.hpp>
#include <lingo/page/unicode.hpp>
#include <lingo/platform/warnings.hpp>

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
		typename DestinationEncoding, typename DestinationPage,
		typename ErrorHandler = error::strict<SourceEncoding, SourcePage, DestinationEncoding, DestinationPage>>
	class string_converter
	{
		public:
		using source_encoding_type = SourceEncoding;
		using source_page_type = SourcePage;
		using source_unit_type = typename source_encoding_type::unit_type;
		using source_point_type = typename source_encoding_type::point_type;

		using destination_encoding_type = DestinationEncoding;
		using destination_page_type = DestinationPage;
		using destination_unit_type = typename destination_encoding_type::unit_type;
		using destination_point_type = typename destination_encoding_type::point_type;

		using error_handler_type = ErrorHandler;
		using point_mapper = page::point_mapper<source_page_type, destination_page_type>;

		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;

		static_assert(std::is_same<typename source_page_type::point_type, typename source_encoding_type::point_type>::value, "source_page_type::point_type must be the same type as source_encoding_type::point_type");
		static_assert(std::is_same<typename destination_page_type::point_type, typename destination_encoding_type::point_type>::value, "destination_page_type::point_type must be the same type as destination_encoding_type::point_type");

		LINGO_CONSTEXPR14 conversion_result convert(
			const source_unit_type* source_buffer, size_type source_buffer_size,
			destination_unit_type* destination_buffer, size_type destination_buffer_size)
		{
			auto read_ptr = source_buffer;
			auto read_size = source_buffer_size;
			auto write_ptr = destination_buffer;
			auto write_size = destination_buffer_size;
			
			while (read_size > 0 && write_size > 0)
			{
				// Try to decode a point from the source
				const auto decode_result = source_encoding_type::decode_point(read_ptr, read_size);
				if (decode_result.error != error::error_code::success)
				{
					if (!handle_error(read_ptr, read_size, write_ptr, write_size, decode_result.error))
					{
						break;
					}
				}

				// Try to map the source point to a destination point
				const auto map_result = point_mapper::map(decode_result.point);
				if (map_result.error != error::error_code::success)
				{
					if (!handle_error(read_ptr, read_size, write_ptr, write_size, map_result.error))
					{
						break;
					}
				}

				// Try to encode the point into the destination buffer
				const auto encode_result = destination_encoding_type::encode_point(map_result.point, write_ptr, write_size);
				if (encode_result.error != error::error_code::success)
				{
					// A destination buffer that is too small is not considered an error.
					// Return from this function and allow the callee to provide more buffer space
					if (encode_result.error == error::error_code::buffer_too_small)
					{
						break;
					}
					else if (!handle_error(read_ptr, read_size, write_ptr, write_size, encode_result.error))
					{
						break;
					}
				}

				read_ptr += decode_result.size;
				read_size -= decode_result.size;
				write_ptr += encode_result.size;
				write_size -= encode_result.size;
			}

			return { source_buffer_size - read_size, destination_buffer_size - write_size };
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
					source.data() + total_units_read, source.size() - total_units_read,
					string.data() + total_units_written, string.size() - total_units_written);

				total_units_read += result.units_read;
				total_units_written += result.units_written;

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
		bool handle_error(
			const source_unit_type*& read_ptr, size_type& read_size,
			destination_unit_type*& write_ptr, size_type& write_size,
			error::error_code err)
		{
			// Attempt to handle the error
			const auto error_handle_result = error_handler_type::handle(read_ptr, read_size, write_ptr, write_size, err);

			// Assume that the error handler was unable to handle the error with the currently available data when no units are read or written
			if (error_handle_result.units_read == 0 && error_handle_result.units_written == 0)
			{
				return false;
			}

			// Error handled, continue and try again
			assert(error_handle_result.units_read <= read_size);
			read_ptr += error_handle_result.units_read;
			read_size -= error_handle_result.units_read;

			assert(error_handle_result.units_written <= write_size);
			write_ptr += error_handle_result.units_written;
			write_size -= error_handle_result.units_written;

			return true;
		}
		LINGO_WARNINGS_POP_MSVC
	};
}

#endif