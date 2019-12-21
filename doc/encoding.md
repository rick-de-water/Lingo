```c++
template <typename UnitT, typename PointT>
struct encoding
{
	using unit_type = UnitT;
	using point_type = PointT;

	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;

	static constexpr size_type max_point_size;

	static constexpr size_type point_size(point_type point) noexcept;
	static constexpr size_type unit_size(unit_type unit) noexcept;

	static constexpr encode_result encode_point(point_type point, unit_type buffer. size_type buffer_size) noexcept;
	static constexpr decode_result<point_type> decode_point(const unit_type* buffer, buffer_size) noexcept;
}
```