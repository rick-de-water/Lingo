# Interfaces

## Encoding
```c++
template <typename UnitT, typename PointT>
struct <my_encoding>
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
	// - none::max_units = 1;
	static constexpr size_type max_units;

	// Stuff below here is probably going to change, so no detailed explainations yet.
	static constexpr size_type point_size(point_type point) noexcept;
	static constexpr size_type unit_size(unit_type unit) noexcept;

	static constexpr encode_result encode_point(point_type point, unit_type buffer. size_type buffer_size) noexcept;
	static constexpr decode_result<point_type> decode_point(const unit_type* buffer, buffer_size) noexcept;
}
```

## Code page
```c++
struct <my_code_page>
{
	// The type of a code point, usually some type of integer
	// TODO: describe constraints
	using point_type = <my_code_point>;

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
}
```