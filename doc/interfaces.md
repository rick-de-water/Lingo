# Interfaces

## Encoding
```c++
template <typename UnitT, typename PointT>
struct my_encoding
{
	// The type of a code unit and a code point
	// When used in a string, unit_type must match the value_type of the allocator.
	// When used in a string, point_type must match the point_type of the code page.
	using unit_type = UnitT;
	using point_type = PointT;

	// Some extra common typedefs
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;

	// The maximum amount of units that are needed to be able to store every valid code point.
	// Examples:
	// - utf8::max_units = 4;
	// - utf16::max_units = 2;
	// - utf32::max_units = 1;
	// - none::max_units = 1;
	static constexpr size_type max_units;

	// Input and output types for the encoding and decoding functions
	using encode_result_type = encode_result<unit_type, point_type>;
	using decode_result_type = decode_result<unit_type, point_type>;
	using encode_source_type = typename encode_result_type::source_type;
	using decode_source_type = typename decode_result_type::source_type;
	using encode_destination_type = typename encode_result_type::destination_type;
	using decode_destination_type = typename decode_result_type::destination_type;
	
	// Objects that keep track of the parse state between encode/decode calls
	using encode_state_type = /* implementation defined */
	using decode_state_type = /* implementation defined */

	// Encode exactly one point from the source buffer and 
	// store the resulting units in the destination buffer
	// The final parameter is used to indicate that there will be no more source data after this
	static constexpr encode_result_type encode_one(encode_source_type source, encode_destination_type destination, encode_state_type& state, bool final) noexcept;

	// Decode exactly one point from the source buffer and 
	// store the resulting point in the destination buffer
	// The final parameter is used to indicate that there will be no more source data after this
	static constexpr decode_result_type decode_one(decode_source_type source, decode_destination_type destination, decode_state_type& state, bool final) noexcept;
}
```

## Code page
```c++
struct my_code_page
{
	// The type of a code point, usually some type of integer
	// TODO: describe constraints
	using point_type = /* implementation defined */;

	// A class that contains information about a code point.
	// Available information may be differ between code pages.
	// TODO: Add mandatory info and interface coding standard.
	using point_info_type = /* implementation defined */

	// Some extra common typedefs
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;

	// The range between which all valid code points exist.
	// This does not mean that all points within this range have to be valid.
	// Examples:
	// - ascii::point_range = 128
	// - iso_8859_1::point_range = 256
	// - unicode::point_range = 0x110000
	static constexpr size_type point_range;

	// Converts a SourcePage code point to a code point of this code page.
	// Only unicode is required to be supported, the rest is optional.
	template <typename SourcePage>
	static constexpr map_result<point_type> map_from(typename SourcePage::point_type point) noexcept;
	
	// Converts a code point of this code page to a SourcePage code point.
	// Only unicode is required to be supported, the rest is optional.
	template <typename SourcePage>
	static constexpr map_result<typename DestinationPage::point_type> map_to(point_type point) noexcept;

	// Checks if a code point is a valid one.
	static bool is_valid(point_type point) noexcept;

	// Returns information about a code point.
	// Throws std::out_of_range if the code point is not a valid one. 
	static const point_info_type& info(point_type point);
}
```