#include <catch/catch.hpp>

#include <lingo/string_view.hpp>

#include <test_case.hpp>

#include <type_traits>

LINGO_TEST_CASE("string_view has the correct exception specifications")
{
	LINGO_TEST_TYPEDEFS;

	REQUIRE(noexcept(::new(std::nothrow) string_view()));
	REQUIRE(noexcept(::new(std::nothrow) string_view(std::declval<const string_view&>())));
	REQUIRE(noexcept(::new(std::nothrow) string_view(std::declval<string_view&&>())));
	REQUIRE(noexcept(::new(std::nothrow) string_view(std::declval<const unit_type*>())));
	REQUIRE(noexcept(::new(std::nothrow) string_view(std::declval<const unit_type*>(), std::declval<size_type>(), std::declval<bool>())));
	REQUIRE(std::is_nothrow_default_constructible<string_view>::value);

	REQUIRE(noexcept(std::declval<string_view&>() = std::declval<const string_view&>()));
	REQUIRE(noexcept(std::declval<string_view&>() = std::declval<string_view&&>()));

	REQUIRE(noexcept(std::declval<string_view&>().begin()));
	REQUIRE(noexcept(std::declval<string_view&>().end()));
	REQUIRE(noexcept(std::declval<string_view&>().cbegin()));
	REQUIRE(noexcept(std::declval<string_view&>().cend()));
	REQUIRE(noexcept(std::declval<string_view&>().rbegin()));
	REQUIRE(noexcept(std::declval<string_view&>().rend()));
	REQUIRE(noexcept(std::declval<string_view&>().crbegin()));
	REQUIRE(noexcept(std::declval<string_view&>().crend()));

	REQUIRE(noexcept(std::declval<string_view&>()[std::declval<size_type>()]));
	REQUIRE_FALSE(noexcept(std::declval<string_view&>().at(std::declval<size_type>())));

	REQUIRE(noexcept(std::declval<string_view&>().front()));
	REQUIRE(noexcept(std::declval<string_view&>().back()));
	REQUIRE(noexcept(std::declval<string_view&>().data()));
	REQUIRE(noexcept(std::declval<string_view&>().size()));
	REQUIRE(noexcept(std::declval<string_view&>().max_size()));
	REQUIRE(noexcept(std::declval<string_view&>().empty()));
	REQUIRE(noexcept(std::declval<string_view&>().null_terminated()));
}

LINGO_TEST_CASE("string_view has the correct types")
{
	LINGO_TEST_TYPEDEFS;

	// Typedefs
	REQUIRE(std::is_same<encoding_type, typename string_view::encoding_type>::value);
	REQUIRE(std::is_same<page_type, typename string_view::page_type>::value);

	REQUIRE(std::is_same<unit_type, typename string_view::value_type>::value);
	REQUIRE(std::is_same<unit_type&, typename string_view::reference>::value);
	REQUIRE(std::is_same<const unit_type&, typename string_view::const_reference>::value);
	REQUIRE(std::is_same<unit_type*, typename string_view::pointer>::value);
	REQUIRE(std::is_same<const unit_type*, typename string_view::const_pointer>::value);

	REQUIRE(std::is_same<size_type, typename string_view::size_type>::value);
	REQUIRE(std::is_same<difference_type, typename string_view::difference_type>::value);

	REQUIRE(std::is_same<typename string_view::iterator, typename string_view::const_iterator>::value);
	REQUIRE(std::is_same<typename string_view::reverse_iterator, typename string_view::const_reverse_iterator>::value);

	// Return values
	REQUIRE(std::is_same<typename string_view::iterator, decltype(std::declval<string_view>().begin())>::value);
	REQUIRE(std::is_same<typename string_view::iterator, decltype(std::declval<const string_view>().begin())>::value);
	REQUIRE(std::is_same<typename string_view::iterator, decltype(std::declval<string_view>().end())>::value);
	REQUIRE(std::is_same<typename string_view::iterator, decltype(std::declval<const string_view>().end())>::value);
	REQUIRE(std::is_same<typename string_view::const_iterator, decltype(std::declval<string_view>().cbegin())>::value);
	REQUIRE(std::is_same<typename string_view::const_iterator, decltype(std::declval<const string_view>().cbegin())>::value);
	REQUIRE(std::is_same<typename string_view::const_iterator, decltype(std::declval<string_view>().cend())>::value);
	REQUIRE(std::is_same<typename string_view::const_iterator, decltype(std::declval<const string_view>().cend())>::value);
	REQUIRE(std::is_same<typename string_view::reverse_iterator, decltype(std::declval<string_view>().rbegin())>::value);
	REQUIRE(std::is_same<typename string_view::reverse_iterator, decltype(std::declval<const string_view>().rbegin())>::value);
	REQUIRE(std::is_same<typename string_view::reverse_iterator, decltype(std::declval<string_view>().rend())>::value);
	REQUIRE(std::is_same<typename string_view::reverse_iterator, decltype(std::declval<const string_view>().rend())>::value);
	REQUIRE(std::is_same<typename string_view::const_reverse_iterator, decltype(std::declval<string_view>().crbegin())>::value);
	REQUIRE(std::is_same<typename string_view::const_reverse_iterator, decltype(std::declval<const string_view>().crbegin())>::value);
	REQUIRE(std::is_same<typename string_view::const_reverse_iterator, decltype(std::declval<string_view>().crend())>::value);
	REQUIRE(std::is_same<typename string_view::const_reverse_iterator, decltype(std::declval<const string_view>().crend())>::value);

	REQUIRE(std::is_same<typename string_view::const_reference, decltype(std::declval<string_view>().front())>::value);
	REQUIRE(std::is_same<typename string_view::const_reference, decltype(std::declval<const string_view>().front())>::value);
	REQUIRE(std::is_same<typename string_view::const_reference, decltype(std::declval<string_view>().back())>::value);
	REQUIRE(std::is_same<typename string_view::const_reference, decltype(std::declval<const string_view>().back())>::value);

	REQUIRE(std::is_same<typename string_view::const_pointer, decltype(std::declval<string_view>().data())>::value);
	REQUIRE(std::is_same<typename string_view::const_pointer, decltype(std::declval<const string_view>().data())>::value);
	REQUIRE(std::is_same<typename string_view::size_type, decltype(std::declval<string_view>().size())>::value);
	REQUIRE(std::is_same<typename string_view::size_type, decltype(std::declval<const string_view>().size())>::value);
	REQUIRE(std::is_same<typename string_view::size_type, decltype(std::declval<string_view>().max_size())>::value);
	REQUIRE(std::is_same<typename string_view::size_type, decltype(std::declval<const string_view>().max_size())>::value);
	REQUIRE(std::is_same<bool, decltype(std::declval<string_view>().empty())>::value);
	REQUIRE(std::is_same<bool, decltype(std::declval<const string_view>().empty())>::value);
	REQUIRE(std::is_same<bool, decltype(std::declval<string_view>().null_terminated())>::value);
	REQUIRE(std::is_same<bool, decltype(std::declval<const string_view>().null_terminated())>::value);
}

LINGO_TEST_CASE("A default constructed string_view is an empty null terminated string")
{
	LINGO_TEST_TYPEDEFS;

	LINGO_CONSTEXPR11 string_view sv;
	REQUIRE(sv.size() == 0);
	REQUIRE(sv.null_terminated());
	REQUIRE(sv.data()[0] == unit_type{});
}

LINGO_TEST_CASE("string_view can be constructed from a cstring")
{
	LINGO_TEST_TYPEDEFS;
	
	for (auto& test_string : lingo::test::generate_test_strings<encoding_type>())
	{
		const string_view test_string_view1(test_string.data);
		REQUIRE(test_string_view1.size() == test_string.size);
		REQUIRE(test_string_view1.data() == test_string.data);
		REQUIRE(test_string_view1.null_terminated());

		const string_view test_string_view2(test_string.data, test_string.size, true);
		REQUIRE(test_string_view2.size() == test_string.size);
		REQUIRE(test_string_view2.data() == test_string.data);
		REQUIRE(test_string_view2.null_terminated());

		const string_view test_string_view3(test_string.data, test_string.size, false);
		REQUIRE(test_string_view3.size() == test_string.size);
		REQUIRE(test_string_view3.data() == test_string.data);
		REQUIRE_FALSE(test_string_view3.null_terminated());

		const string_view test_string_view4(test_string.data, test_string.size / 2, false);
		REQUIRE(test_string_view4.size() == test_string.size / 2);
		REQUIRE(test_string_view4.data() == test_string.data);
		REQUIRE_FALSE(test_string_view4.null_terminated());
	}
}

LINGO_TEST_CASE("string_view can be copied")
{
	LINGO_TEST_TYPEDEFS;

	for (auto& test_string : lingo::test::generate_test_strings<encoding_type>())
	{
		const string_view test_string_view1(test_string.data);
		string_view test_string_view2(test_string_view1);

		REQUIRE(test_string_view1.size() == test_string_view2.size());
		REQUIRE(test_string_view1.data() == test_string_view2.data());
		REQUIRE(test_string_view1.null_terminated() == test_string_view1.null_terminated());

		const string_view test_string_view3(test_string_view1.data(), test_string_view1.size(), false);
		test_string_view2 = test_string_view3;

		REQUIRE(test_string_view3.size() == test_string_view2.size());
		REQUIRE(test_string_view3.data() == test_string_view2.data());
		REQUIRE(test_string_view3.null_terminated() == test_string_view2.null_terminated());
	}
}

LINGO_TEST_CASE("string_view can be iterated over")
{
	LINGO_TEST_TYPEDEFS;

	for (auto& test_string : lingo::test::generate_test_strings<encoding_type>())
	{
		const string_view test_string_view(test_string.data);
		size_t index;

		index = 0;
		for (; index < test_string.size; ++index)
		{
			REQUIRE(test_string_view[index] == test_string.data[index]);
			REQUIRE(test_string_view.at(index) == test_string.data[index]);
		}
		REQUIRE_THROWS(test_string_view.at(index));

		index = 0;
		for (auto& c : test_string_view)
		{
			REQUIRE(c == test_string.data[index]);
			++index;
		}

		index = 0;
		for (auto it = test_string_view.begin(); it != test_string_view.end(); ++it)
		{
			REQUIRE(*it == test_string.data[index]);
			++index;
		}

		index = 0;
		for (auto it = test_string_view.cbegin(); it != test_string_view.cend(); ++it)
		{
			REQUIRE(*it == test_string.data[index]);
			++index;
		}

		index = test_string.size - 1;
		for (auto it = test_string_view.rbegin(); it != test_string_view.rend(); ++it)
		{
			REQUIRE(*it == test_string.data[index]);
			--index;
		}

		index = test_string.size - 1;
		for (auto it = test_string_view.crbegin(); it != test_string_view.crend(); ++it)
		{
			REQUIRE(*it == test_string.data[index]);
			--index;
		}
	}
}

LINGO_TEST_CASE("string_view can be empty")
{
	LINGO_TEST_TYPEDEFS;

	for (auto& test_string : lingo::test::generate_test_strings<encoding_type>())
	{
		const string_view test_string_view(test_string.data);

		if (test_string.size == 0)
		{
			REQUIRE(test_string_view.empty());
		}
		else
		{
			REQUIRE(test_string_view.front() == test_string.data[0]);
			REQUIRE(test_string_view.back() == test_string.data[test_string.size - 1]);
		}
	}
}

LINGO_TEST_CASE("string_view has a reasonable maximum size")
{
	LINGO_TEST_TYPEDEFS;

	const string_view test_string_view;
	REQUIRE(test_string_view.max_size() >= 1024 * 1024 * 1024);
}

LINGO_TEST_CASE("string_view can have prefixes and suffixes removed")
{
	LINGO_TEST_TYPEDEFS;

	for (auto& test_string : lingo::test::generate_test_strings<encoding_type>())
	{
		for (size_t i = 1; i < 10; ++i)
		{
			const size_t n = test_string.size / i;

			string_view test_string_view(test_string.data);
			string_view prefix = test_string_view.remove_prefix(n);

			REQUIRE(prefix.data() == test_string.data);
			REQUIRE(prefix.size() == n);

			REQUIRE(test_string_view.data() == test_string.data + n);
			REQUIRE(test_string_view.size() == test_string.size - n);
		}

		for (size_t i = 1; i < 10; ++i)
		{
			const size_t n = test_string.size / i;

			string_view test_string_view(test_string.data);
			string_view suffix = test_string_view.remove_suffix(n);

			REQUIRE(suffix.data() == test_string.data + (test_string.size - n));
			REQUIRE(suffix.size() == n);

			REQUIRE(test_string_view.data() == test_string.data);
			REQUIRE(test_string_view.size() == test_string.size - n);
		}
	}
}

LINGO_TEST_CASE("string_view can be compared")
{
	LINGO_TEST_TYPEDEFS;

	string_view prev_test_string_view;

	bool first = true;
	for (auto& test_string : lingo::test::generate_test_strings<encoding_type>())
	{
		if (first)
		{
			prev_test_string_view = string_view(test_string.data);
			first = false;
			continue;
		}

		string_view test_string_view(test_string.data);
		REQUIRE(test_string_view == test_string_view);
		REQUIRE_FALSE(test_string_view != test_string_view);

		REQUIRE_FALSE(test_string_view == prev_test_string_view);
		REQUIRE(test_string_view != prev_test_string_view);

		prev_test_string_view = test_string_view;
	}
}

LINGO_TEST_CASE("string_view can be swapped")
{
	LINGO_TEST_TYPEDEFS;

	for (auto& test_string : lingo::test::generate_test_strings<encoding_type>())
	{
		string_view test_string_view1(test_string.data);
		string_view test_string_view2(test_string_view1);

		test_string_view2.remove_prefix(test_string.size / 4);
		test_string_view2.remove_suffix(test_string.size / 4);

		string_view test_string_view3(test_string_view1);
		string_view test_string_view4(test_string_view2);

		REQUIRE(test_string_view1 == test_string_view3);
		REQUIRE(test_string_view2 == test_string_view4);
		test_string_view3.swap(test_string_view4);
		REQUIRE(test_string_view1 == test_string_view4);
		REQUIRE(test_string_view2 == test_string_view3);
	}
}