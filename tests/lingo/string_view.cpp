#include <catch/catch.hpp>

#if LINGO_TEST_SPLIT
#include <lingo/string_view.hpp>
#else
#include <lingo/test/include_all.hpp>
#endif

#include <lingo/test/test_case.hpp>
#include <lingo/test/test_strings.hpp>

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

TEST_CASE("lingo::string_view can be constructed from a std::string")
{
	auto str1 = std::basic_string<char>("foo");
	auto str2 = std::basic_string<char8_t>(u8"bar");
	auto str_view1 = lingo::string_view(str1);
	auto str_view2 = lingo::string_view(str2);

	REQUIRE(str_view1 == "foo");
	REQUIRE(str_view2 == "bar");
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

TEST_CASE("string_view can be searched")
{
	using string_view_type = lingo::string_view;
	using size_type = string_view_type::size_type;

	const string_view_type text(
		"ctataatcccagcttgttgggaggccaaggcaggaggatcacttgaagcc"
		"caggagtttgagacgagcctaagcaacatagcaagaccctatctctacaa"
		"ttataaatatagtatttgttaatatttggccaggcgtggtagtacatgcc"
		"tgtaggcccagctacttggggagaggaggcaggaggatcacttgagggcc"
		"gaagttctgggctgtagtgcactatgcaatcaggtgtccccaccacattg"
		"ggcataaatctggtgatttcctgggagaaggagaccaccaggttgcctta"
		"taaggggtaaactggcccacatcgaaaacagagtaggtcaacatttccat"
		"gctaatcagtaatgggatccagcctatgaataaccactgcactccagctc"
		"gggcaatacagcaagaccctgaatttttttttttttttttttttgagaca"
		"gtcttgctctgtggcccaggatggagtgcaatggtgtgatctcagatcac"
		"tacaacctccacctccgggattcaagcaattcttctgcttcagcctccca"
		"agtagctgggattacaggtgggcgccaccacacctggccgtacccacatt"
		"ttttaaaaattaaaaaaataagaataaggccaggcacggtggctcaccct"
		"tgtaatcccagcactttgggaggccaaggtgggcagatcacctgaggtca"
		"aaagttcgagaccagcatggccaacatggcaaaaccctgtctctaataaa"
		"aatacaaaaattagccaggcgtggtggcgggcacctgtaatcccagctac"
		"tcgggaggctgagacaggagaattgcttgaacccaggaactggaggtttc"
		"agtgagccaagattgcgccattgcactccagcctgggcaacaagagtgaa"
		"gctccatctcaaaaaaaaaaaaaaaatttagcctggcgtggtggcacgtg"
		"cctgtagtcccagctaccagggagtttggggttggaggattgcttgagcc"
		"cgggaagcagaggttgcagtgagctgagatcctgccaccacactccagcc"
		"tgggctacagagtgagacactgtctcaaaaaaataaaaaataaaaataaa"
		"atgtaaaaaaatctgatgaacatcattccaatgtcaattataaattgttt"
		"gatgttaggtatttatttatttatttattttattttttatttttatttat"
		"ttatttgttttttttgagatggagtttcactcgttgcccaggctagagtg"
		"caatggcgcgatgtcggctcaccacaacctccacctcccgtgttcaagcg"
		"attctcctgcctcggcctcctgagtagctgggattacaggcatgcgccac"
		"cacgcccagctaattttgtacttttagtagagacgtggtttctccatgtt"
		"ggtcaggctggtcttgctggtctcaaactccctgacctcaagtgatccac"
		"ccgccttggcctcccaaagtgctgggattacaggtgtgagccaccacgcc"
		"tggcctatgttaggtatttaatataaatgcctttcatatttatagaaaag"
		"ccattccagctccccacctcttccaatggtcctagagaggtagggggcaa"
		"gactcaactcaggaggtggggctcagaaataggaccaagttgacgactag"
		"ctaaaacagggacggaagagaagcagctttccatgacatgcccaacagtg"
		"tgccctgtcagttcaccattgccatggcaacactgggatgtttccgcccc"
		"tttccattgcaacaacctgatgacctggaaattaccaaccttttcctaga"
		"aatttctgcatagcccgcttcttaatttgcatgtaattaaaagtgagtta"
		"taggctgggcgagttggctcacgcctgtaatcccggcactttgggaggcc"
		"gagacgggcggatcacttgaggtcaggagttccagaccagcctggccaac"
		"atggtaaaaccccatctctacgaaaaatataaaaattacccaggagtggt"
		"gctgcacgcctgtaatcccagctactccagaggctgagtcaggagaatcg"
		"cttgaacccaggaggcagtgagctgagatctcatcactgcactccagcct"
		"aggtgacagagcaagactccatctcagaaaacaaaaacaacaacaacaac"
		"aaaaaaagtgagttataaatatgactgcaggctgggcatggtggctgacg"
		"cctgtaatcccagcactttgggaggccaaggtgggtggatcatgaggtca"
		"ggagatcgagaccatcctgactaacatggtgaagccccgtctctactaaa"
		"aatacaaaaaattagccaggcatggtggcgggtggctatagtcccagcta");

	auto find = [&](string_view_type pattern, std::initializer_list<size_type> matches)
	{
		size_type offset = 0;
		for (size_type expected_match : matches)
		{
			const size_type match = text.find(pattern, offset);

			REQUIRE(expected_match == match);

			offset = expected_match + 1;
		}

		REQUIRE(text.find(pattern, offset) == string_view_type::npos);
	};

	auto rfind = [&](string_view_type pattern, std::initializer_list<size_type> matches)
	{
		size_type offset = string_view_type::npos;
		for (size_type expected_match : matches)
		{
			const size_type match = text.rfind(pattern, offset);

			REQUIRE(expected_match == match);

			offset = expected_match + pattern.size() - 2;
		}

		REQUIRE(text.rfind(pattern, offset) == string_view_type::npos);
	};

	REQUIRE(text.find("c") == 0);
	REQUIRE(text.find("t") == 1);
	REQUIRE(text.find("a") == 2);
	REQUIRE(text.find("g") == 11);
	REQUIRE(text.find("x") == string_view_type::npos);
	REQUIRE(text.find(text) == 0);
	REQUIRE(text.find(text + "c") == string_view_type::npos);

	find("agaa", { 275, 620, 818, 1543, 1624, 1668, 1797, 2043, 2125 });
	find("tttttt", { 423, 424, 425, 426, 427, 428, 429, 430, 431, 432, 433, 434, 435, 436, 437, 438, 598, 1182, 1207, 1208, 1209 });
	find("ctga", { 418, 691, 808, 1023, 1112, 1319, 1431, 1766, 2033, 2072, 2194, 2266 });

	REQUIRE(text.rfind("c") == 2347);
	REQUIRE(text.rfind("t") == 2348);
	REQUIRE(text.rfind("a") == 2349);
	REQUIRE(text.rfind("g") == 2346);
	REQUIRE(text.rfind("x") == string_view_type::npos);
	REQUIRE(text.rfind(text) == 0);
	REQUIRE(text.rfind(text + "c") == string_view_type::npos);

	rfind("agaa", { 2125, 2043, 1797, 1668, 1624, 1543, 818, 620, 275 });
	rfind("tttttt", { 1209, 1208, 1207, 1182, 598, 438, 437, 436, 435, 434, 433, 432, 431, 430, 429, 428, 427, 426, 425, 424, 423 });
	rfind("ctga", { 2266, 2194, 2072, 2033, 1766, 1431, 1319, 1112, 1023, 808, 691, 418 });

	REQUIRE(text.starts_with("c"));
	REQUIRE_FALSE(text.starts_with("t"));
	REQUIRE_FALSE(text.starts_with("a"));
	REQUIRE_FALSE(text.starts_with("g"));

	REQUIRE_FALSE(text.starts_with("cc"));
	REQUIRE(text.starts_with("ct"));
	REQUIRE_FALSE(text.starts_with("ca"));
	REQUIRE_FALSE(text.starts_with("cg"));

	REQUIRE_FALSE(text.starts_with("ctc"));
	REQUIRE_FALSE(text.starts_with("ctt"));
	REQUIRE(text.starts_with("cta"));
	REQUIRE_FALSE(text.starts_with("ctg"));

	REQUIRE(text.starts_with("ctataatcccagcttgttgggaggccaaggcaggaggatcacttgaagcc"));
	REQUIRE_FALSE(text.starts_with("ctataatcccagcttgttgggaggccaaggccggaggatcacttgaagcc"));

	REQUIRE(text.starts_with(string_view_type("ctataatcccagcttgttgggaggccaaggcaggaggatcacttgaagcc")));
	REQUIRE_FALSE(text.starts_with(string_view_type("ctataatcccagcttgttgggaggccaaggccggaggatcacttgaagcc")));


	REQUIRE_FALSE(text.ends_with("c"));
	REQUIRE_FALSE(text.ends_with("t"));
	REQUIRE(text.ends_with("a"));
	REQUIRE_FALSE(text.ends_with("g"));

	REQUIRE_FALSE(text.ends_with("ca"));
	REQUIRE(text.ends_with("ta"));
	REQUIRE_FALSE(text.ends_with("aa"));
	REQUIRE_FALSE(text.ends_with("ga"));

	REQUIRE(text.ends_with("cta"));
	REQUIRE_FALSE(text.ends_with("tta"));
	REQUIRE_FALSE(text.ends_with("ata"));
	REQUIRE_FALSE(text.ends_with("gta"));

	REQUIRE(text.ends_with("aatacaaaaaattagccaggcatggtggcgggtggctatagtcccagcta"));
	REQUIRE_FALSE(text.ends_with("aatacaaaaaattagccaggcatggtggctggtggctatagtcccagcta"));

	REQUIRE(text.ends_with(string_view_type("aatacaaaaaattagccaggcatggtggcgggtggctatagtcccagcta")));
	REQUIRE_FALSE(text.ends_with(string_view_type("aatacaaaaaattagccaggcatggtggctggtggctatagtcccagcta")));


}