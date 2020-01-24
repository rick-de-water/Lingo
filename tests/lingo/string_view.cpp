#include <catch/catch.hpp>

#include <lingo/string_view.hpp>

#include "test_case.hpp"
#include "test_strings.hpp"

#include <type_traits>

LINGO_UNIT_TEST_CASE("string_view has the correct exception specifications")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	REQUIRE(noexcept(::new(std::nothrow) string_view_type()));
	REQUIRE(noexcept(::new(std::nothrow) string_view_type(std::declval<const string_view_type&>())));
	REQUIRE(noexcept(::new(std::nothrow) string_view_type(std::declval<string_view_type&&>())));
	REQUIRE(noexcept(::new(std::nothrow) string_view_type(std::declval<const unit_type*>())));
	REQUIRE(noexcept(::new(std::nothrow) string_view_type(std::declval<const unit_type*>(), std::declval<size_type>(), std::declval<bool>())));
	REQUIRE(std::is_nothrow_default_constructible<string_view_type>::value);

	REQUIRE(noexcept(std::declval<string_view_type&>() = std::declval<const string_view_type&>()));
	REQUIRE(noexcept(std::declval<string_view_type&>() = std::declval<string_view_type&&>()));

	REQUIRE(noexcept(std::declval<string_view_type&>().begin()));
	REQUIRE(noexcept(std::declval<string_view_type&>().end()));
	REQUIRE(noexcept(std::declval<string_view_type&>().cbegin()));
	REQUIRE(noexcept(std::declval<string_view_type&>().cend()));
	REQUIRE(noexcept(std::declval<string_view_type&>().rbegin()));
	REQUIRE(noexcept(std::declval<string_view_type&>().rend()));
	REQUIRE(noexcept(std::declval<string_view_type&>().crbegin()));
	REQUIRE(noexcept(std::declval<string_view_type&>().crend()));

	REQUIRE(noexcept(std::declval<string_view_type&>()[std::declval<size_type>()]));
	REQUIRE_FALSE(noexcept(std::declval<string_view_type&>().at(std::declval<size_type>())));

	REQUIRE(noexcept(std::declval<string_view_type&>().front()));
	REQUIRE(noexcept(std::declval<string_view_type&>().back()));
	REQUIRE(noexcept(std::declval<string_view_type&>().data()));
	REQUIRE(noexcept(std::declval<string_view_type&>().size()));
	REQUIRE(noexcept(std::declval<string_view_type&>().max_size()));
	REQUIRE(noexcept(std::declval<string_view_type&>().empty()));
	REQUIRE(noexcept(std::declval<string_view_type&>().null_terminated()));
}

LINGO_UNIT_TEST_CASE("string_view has the correct types")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	// Typedefs
	REQUIRE(std::is_same<encoding_type, typename string_view_type::encoding_type>::value);
	REQUIRE(std::is_same<page_type, typename string_view_type::page_type>::value);

	REQUIRE(std::is_same<unit_type, typename string_view_type::value_type>::value);
	REQUIRE(std::is_same<unit_type&, typename string_view_type::reference>::value);
	REQUIRE(std::is_same<const unit_type&, typename string_view_type::const_reference>::value);
	REQUIRE(std::is_same<unit_type*, typename string_view_type::pointer>::value);
	REQUIRE(std::is_same<const unit_type*, typename string_view_type::const_pointer>::value);

	REQUIRE(std::is_same<size_type, typename string_view_type::size_type>::value);
	REQUIRE(std::is_same<difference_type, typename string_view_type::difference_type>::value);

	REQUIRE(std::is_same<typename string_view_type::iterator, typename string_view_type::const_iterator>::value);
	REQUIRE(std::is_same<typename string_view_type::reverse_iterator, typename string_view_type::const_reverse_iterator>::value);

	// Return values
	REQUIRE(std::is_same<typename string_view_type::iterator, decltype(std::declval<string_view_type>().begin())>::value);
	REQUIRE(std::is_same<typename string_view_type::iterator, decltype(std::declval<const string_view_type>().begin())>::value);
	REQUIRE(std::is_same<typename string_view_type::iterator, decltype(std::declval<string_view_type>().end())>::value);
	REQUIRE(std::is_same<typename string_view_type::iterator, decltype(std::declval<const string_view_type>().end())>::value);
	REQUIRE(std::is_same<typename string_view_type::const_iterator, decltype(std::declval<string_view_type>().cbegin())>::value);
	REQUIRE(std::is_same<typename string_view_type::const_iterator, decltype(std::declval<const string_view_type>().cbegin())>::value);
	REQUIRE(std::is_same<typename string_view_type::const_iterator, decltype(std::declval<string_view_type>().cend())>::value);
	REQUIRE(std::is_same<typename string_view_type::const_iterator, decltype(std::declval<const string_view_type>().cend())>::value);
	REQUIRE(std::is_same<typename string_view_type::reverse_iterator, decltype(std::declval<string_view_type>().rbegin())>::value);
	REQUIRE(std::is_same<typename string_view_type::reverse_iterator, decltype(std::declval<const string_view_type>().rbegin())>::value);
	REQUIRE(std::is_same<typename string_view_type::reverse_iterator, decltype(std::declval<string_view_type>().rend())>::value);
	REQUIRE(std::is_same<typename string_view_type::reverse_iterator, decltype(std::declval<const string_view_type>().rend())>::value);
	REQUIRE(std::is_same<typename string_view_type::const_reverse_iterator, decltype(std::declval<string_view_type>().crbegin())>::value);
	REQUIRE(std::is_same<typename string_view_type::const_reverse_iterator, decltype(std::declval<const string_view_type>().crbegin())>::value);
	REQUIRE(std::is_same<typename string_view_type::const_reverse_iterator, decltype(std::declval<string_view_type>().crend())>::value);
	REQUIRE(std::is_same<typename string_view_type::const_reverse_iterator, decltype(std::declval<const string_view_type>().crend())>::value);

	REQUIRE(std::is_same<typename string_view_type::const_reference, decltype(std::declval<string_view_type>().front())>::value);
	REQUIRE(std::is_same<typename string_view_type::const_reference, decltype(std::declval<const string_view_type>().front())>::value);
	REQUIRE(std::is_same<typename string_view_type::const_reference, decltype(std::declval<string_view_type>().back())>::value);
	REQUIRE(std::is_same<typename string_view_type::const_reference, decltype(std::declval<const string_view_type>().back())>::value);

	REQUIRE(std::is_same<typename string_view_type::const_pointer, decltype(std::declval<string_view_type>().data())>::value);
	REQUIRE(std::is_same<typename string_view_type::const_pointer, decltype(std::declval<const string_view_type>().data())>::value);
	REQUIRE(std::is_same<typename string_view_type::size_type, decltype(std::declval<string_view_type>().size())>::value);
	REQUIRE(std::is_same<typename string_view_type::size_type, decltype(std::declval<const string_view_type>().size())>::value);
	REQUIRE(std::is_same<typename string_view_type::size_type, decltype(std::declval<string_view_type>().max_size())>::value);
	REQUIRE(std::is_same<typename string_view_type::size_type, decltype(std::declval<const string_view_type>().max_size())>::value);
	REQUIRE(std::is_same<bool, decltype(std::declval<string_view_type>().empty())>::value);
	REQUIRE(std::is_same<bool, decltype(std::declval<const string_view_type>().empty())>::value);
	REQUIRE(std::is_same<bool, decltype(std::declval<string_view_type>().null_terminated())>::value);
	REQUIRE(std::is_same<bool, decltype(std::declval<const string_view_type>().null_terminated())>::value);
}

LINGO_UNIT_TEST_CASE("A default constructed string_view is an empty null terminated string")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	const string_view_type sv{};
	REQUIRE(sv.size() == 0);
	REQUIRE(sv.null_terminated());
	REQUIRE(sv.data()[0] == unit_type{});
}

LINGO_UNIT_TEST_CASE("string_view can be constructed from a cstring")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	const unit_type* data = lingo::test::test_string<unit_type>::value;
	const size_type size = sizeof(lingo::test::test_string<unit_type>::value) / sizeof(lingo::test::test_string<unit_type>::value[0]) - 1;

	const string_view_type test_string_view1(data);
	REQUIRE(test_string_view1.size() == size);
	REQUIRE(test_string_view1.data() == data);
	REQUIRE(test_string_view1.null_terminated());

	const string_view_type test_string_view2(data, size, true);
	REQUIRE(test_string_view2.size() == size);
	REQUIRE(test_string_view2.data() == data);
	REQUIRE(test_string_view2.null_terminated());

	const string_view_type test_string_view3(data, size, false);
	REQUIRE(test_string_view3.size() == size);
	REQUIRE(test_string_view3.data() == data);
	REQUIRE_FALSE(test_string_view3.null_terminated());

	const string_view_type test_string_view4(data, size / 2, false);
	REQUIRE(test_string_view4.size() == size / 2);
	REQUIRE(test_string_view4.data() == data);
	REQUIRE_FALSE(test_string_view4.null_terminated());

	const string_view_type test_string_view5 = lingo::test::test_string<unit_type>::value;
	REQUIRE(test_string_view5.size() == size);
	REQUIRE(test_string_view5.data() == data);
	REQUIRE(test_string_view5.null_terminated());
}

LINGO_UNIT_TEST_CASE("string_view can be copied")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	const string_view_type test_string_view1(lingo::test::test_string<unit_type>::value);
	string_view_type test_string_view2(test_string_view1);

	REQUIRE(test_string_view1.size() == test_string_view2.size());
	REQUIRE(test_string_view1.data() == test_string_view2.data());
	REQUIRE(test_string_view1.null_terminated() == test_string_view1.null_terminated());

	const string_view_type test_string_view3(test_string_view1.data(), test_string_view1.size(), false);
	test_string_view2 = test_string_view3;

	REQUIRE(test_string_view3.size() == test_string_view2.size());
	REQUIRE(test_string_view3.data() == test_string_view2.data());
	REQUIRE(test_string_view3.null_terminated() == test_string_view2.null_terminated());
}

LINGO_UNIT_TEST_CASE("string_view can be iterated over")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	const unit_type* data = lingo::test::test_string<unit_type>::value;
	const difference_type size = sizeof(lingo::test::test_string<unit_type>::value) / sizeof(lingo::test::test_string<unit_type>::value[0]) - 1;

	const string_view_type test_string_view(data);
	difference_type index;

	index = 0;
	for (; index < size; ++index)
	{
		REQUIRE(test_string_view[index] == data[index]);
		REQUIRE(test_string_view.at(index) == data[index]);
	}
	REQUIRE_THROWS(test_string_view.at(index));

	index = 0;
	for (auto& c : test_string_view)
	{
		REQUIRE(c == data[index]);
		++index;
	}
	REQUIRE(index == size);

	index = 0;
	for (auto it = test_string_view.begin(); it != test_string_view.end(); ++it)
	{
		REQUIRE(*it == data[index]);
		++index;
	}
	REQUIRE(index == size);

	index = 0;
	for (auto it = test_string_view.cbegin(); it != test_string_view.cend(); ++it)
	{
		REQUIRE(*it == data[index]);
		++index;
	}
	REQUIRE(index == size);

	index = size - 1;
	for (auto it = test_string_view.rbegin(); it != test_string_view.rend(); ++it)
	{
		REQUIRE(*it == data[index]);
		--index;
	}
	REQUIRE(index == -1);

	index = size - 1;
	for (auto it = test_string_view.crbegin(); it != test_string_view.crend(); ++it)
	{
		REQUIRE(*it == data[index]);
		--index;
	}
	REQUIRE(index == -1);
}

LINGO_UNIT_TEST_CASE("string_view can be empty")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	const string_view_type test_string_view1(lingo::test::test_string<unit_type>::value);
	const string_view_type test_string_view2{};

	if (test_string_view1.size() == 0)
	{
		REQUIRE(test_string_view1.empty());
	}
	else
	{
		REQUIRE(test_string_view1.front() == test_string_view1.data()[0]);
		REQUIRE(test_string_view1.back() == test_string_view1.data()[test_string_view1.size() - 1]);
	}

	if (test_string_view2.size() == 0)
	{
		REQUIRE(test_string_view2.empty());
	}
	else
	{
		REQUIRE(test_string_view2.front() == test_string_view2.data()[0]);
		REQUIRE(test_string_view2.back() == test_string_view2.data()[test_string_view2.size() - 1]);
	}
}

LINGO_UNIT_TEST_CASE("string_view has a reasonable maximum size")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	const string_view_type test_string_view{};
	REQUIRE(test_string_view.max_size() >= 1024 * 1024 * 1024);
}

LINGO_UNIT_TEST_CASE("string_view can have prefixes and suffixes removed")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	const unit_type* data = lingo::test::test_string<unit_type>::value;
	const size_type size = sizeof(lingo::test::test_string<unit_type>::value) / sizeof(lingo::test::test_string<unit_type>::value[0]) - 1;

	for (size_t i = 1; i < 10; ++i)
	{
		const size_t n = size / i;

		string_view_type test_string_view(data);
		string_view_type prefix = test_string_view.remove_prefix(n);

		REQUIRE(prefix.data() == data);
		REQUIRE(prefix.size() == n);

		REQUIRE(test_string_view.data() == data + n);
		REQUIRE(test_string_view.size() == size - n);
	}

	for (size_t i = 1; i < 10; ++i)
	{
		const size_t n = size / i;

		string_view_type test_string_view(data);
		string_view_type suffix = test_string_view.remove_suffix(n);

		REQUIRE(suffix.data() == data + (size - n));
		REQUIRE(suffix.size() == n);

		REQUIRE(test_string_view.data() == data);
		REQUIRE(test_string_view.size() == size - n);
	}
}

LINGO_UNIT_TEST_CASE("string_view can be swapped")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	const unit_type* data = lingo::test::test_string<unit_type>::value;
	const size_type size = sizeof(lingo::test::test_string<unit_type>::value) / sizeof(lingo::test::test_string<unit_type>::value[0]) - 1;

	string_view_type test_string_view1(data);
	string_view_type test_string_view2(test_string_view1);

	test_string_view2.remove_prefix(size / 4 + 2);
	test_string_view2.remove_suffix(size / 4 + 2);

	string_view_type test_string_view3(test_string_view1);
	string_view_type test_string_view4(test_string_view2);

	REQUIRE(test_string_view1 == test_string_view3);
	REQUIRE(test_string_view2 == test_string_view4);
	test_string_view3.swap(test_string_view4);
	REQUIRE(test_string_view1 == test_string_view4);
	REQUIRE(test_string_view2 == test_string_view3);
}

LINGO_UNIT_TEST_CASE("string_view can be copied to an array")
{
	LINGO_UNIT_TEST_TYPEDEFS;

	const unit_type* data = lingo::test::test_string<unit_type>::value;
	const size_type size = sizeof(lingo::test::test_string<unit_type>::value) / sizeof(lingo::test::test_string<unit_type>::value[0]) - 1;
	const string_view_type source(data, size, true);

	for (auto pos_it = point_iterator_type(source); pos_it != point_iterator_type(); ++pos_it)
	{
		const size_type pos = pos_it.read_ptr() - source.data();

		for (auto size_it = pos_it; size_it != point_iterator_type(); ++size_it)
		{
			const size_type buffer_size = (size_it.read_ptr() - source.data()) - pos;
			auto buffer = std::unique_ptr<unit_type[]>(new unit_type[buffer_size]);

			const size_type copied_count = source.copy(buffer.get(), buffer_size, pos);

			REQUIRE(copied_count == buffer_size);
			REQUIRE(string_view_type(buffer.get(), buffer_size, false) == source.substr(pos, buffer_size));
		}
	}

	REQUIRE_THROWS_AS(source.copy(nullptr, 0, size + 1), std::out_of_range);
}