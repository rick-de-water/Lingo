#include <catch/catch.hpp>

#if LINGO_TEST_SPLIT
#include <lingo/utility/pointer_iterator.hpp>
#else
#include <lingo/test/include_all.hpp>
#endif

namespace
{
	struct test_struct
	{
		int value;
	};

	struct test_fixture
	{
		test_struct test_values[10]{ { 10 }, { 20 }, { 30 }, { 40 }, { 50 }, { 60 }, { 70 }, { 80 }, { 90 }, { 100 } };
	};

	struct test_pointer_iterator_tag;
	using test_pointer_iterator = lingo::utility::pointer_iterator<test_struct, test_pointer_iterator_tag>;
	using const_test_pointer_iterator = lingo::utility::pointer_iterator<const test_struct, test_pointer_iterator_tag>;
}

TEST_CASE_METHOD(test_fixture, "pointer_iterator has the correct typedefs")
{
	REQUIRE(std::is_same<test_pointer_iterator::iterator_category, std::random_access_iterator_tag>::value);
	REQUIRE(std::is_same<test_pointer_iterator::value_type, test_struct>::value);
	REQUIRE(std::is_same<test_pointer_iterator::difference_type, std::ptrdiff_t>::value);
	REQUIRE(std::is_same<test_pointer_iterator::pointer, test_struct*>::value);
	REQUIRE(std::is_same<test_pointer_iterator::reference, test_struct&>::value);

	REQUIRE(std::is_same<const_test_pointer_iterator::iterator_category, std::random_access_iterator_tag>::value);
	REQUIRE(std::is_same<const_test_pointer_iterator::value_type, const test_struct>::value);
	REQUIRE(std::is_same<const_test_pointer_iterator::difference_type, std::ptrdiff_t>::value);
	REQUIRE(std::is_same<const_test_pointer_iterator::pointer, const test_struct*>::value);
	REQUIRE(std::is_same<const_test_pointer_iterator::reference, const test_struct&>::value);
}

TEST_CASE_METHOD(test_fixture, "pointer_iterator can be created from a pointer and then dereferenced")
{
	test_struct value;
	value.value = 7;

	const test_pointer_iterator it(&value);
	const const_test_pointer_iterator const_it(&value);

	REQUIRE((*it).value == 7);
	REQUIRE((*const_it).value == 7);
	REQUIRE(it->value == 7);
	REQUIRE(const_it->value == 7);

	value.value = 8;
	REQUIRE((*it).value == 8);
	REQUIRE((*const_it).value == 8);
	REQUIRE(it->value == 8);
	REQUIRE(const_it->value == 8);

	(*it).value = 9;
	REQUIRE((*it).value == 9);
	REQUIRE((*const_it).value == 9);
	REQUIRE(it->value == 9);
	REQUIRE(const_it->value == 9);

	it->value = 4;
	REQUIRE((*it).value == 4);
	REQUIRE((*const_it).value == 4);
	REQUIRE(it->value == 4);
	REQUIRE(const_it->value == 4);
}

TEST_CASE_METHOD(test_fixture, "pointer_iterator can be converted to a const version")
{
	REQUIRE(std::is_convertible<test_pointer_iterator, const_test_pointer_iterator>::value);
	REQUIRE_FALSE(std::is_convertible<const_test_pointer_iterator, test_pointer_iterator>::value);
	test_struct value;
	value.value = 7;

	const test_pointer_iterator it(&value);
	const const_test_pointer_iterator const_it = it;

	REQUIRE(it->value == const_it->value);
	REQUIRE(it == const_it);
}

TEST_CASE_METHOD(test_fixture, "pointer_iterator can be compared")
{
	test_pointer_iterator its[4];
	const_test_pointer_iterator const_its[4];

	for (std::size_t i = 0; i < 4; ++i)
	{
		its[i] = test_pointer_iterator(test_values + i / 2);
		const_its[i] = const_test_pointer_iterator(test_values + i / 2);
	}

	for (std::size_t j = 0; j < 4; ++j)
	{
		for (std::size_t i = 0; i < 4; ++i)
		{
			std::size_t left = j / 2;
			std::size_t right = i / 2;

			if (left == right)
			{
				REQUIRE(its[j] == its[i]);
				REQUIRE(its[j] == const_its[i]);
				REQUIRE(const_its[j] == its[i]);
				REQUIRE(const_its[j] == const_its[i]);

				REQUIRE_FALSE(its[j] != its[i]);
				REQUIRE_FALSE(its[j] != const_its[i]);
				REQUIRE_FALSE(const_its[j] != its[i]);
				REQUIRE_FALSE(const_its[j] != const_its[i]);
			}
			else
			{
				REQUIRE(its[j] != its[i]);
				REQUIRE(its[j] != const_its[i]);
				REQUIRE(const_its[j] != its[i]);
				REQUIRE(const_its[j] != const_its[i]);

				REQUIRE_FALSE(its[j] == its[i]);
				REQUIRE_FALSE(its[j] == const_its[i]);
				REQUIRE_FALSE(const_its[j] == its[i]);
				REQUIRE_FALSE(const_its[j] == const_its[i]);
			}
		}
	}
}

TEST_CASE_METHOD(test_fixture, "pointer_iterator can be incremented and decremented")
{
	test_pointer_iterator it(test_values);
	const_test_pointer_iterator const_it(test_values);

	REQUIRE(it == test_pointer_iterator(test_values + 0));
	REQUIRE(++it == test_pointer_iterator(test_values + 1));
	REQUIRE(it == test_pointer_iterator(test_values + 1));
	REQUIRE(it++ == test_pointer_iterator(test_values + 1));
	REQUIRE(it == test_pointer_iterator(test_values + 2));
	REQUIRE(--it == test_pointer_iterator(test_values + 1));
	REQUIRE(it == test_pointer_iterator(test_values + 1));
	REQUIRE(it-- == test_pointer_iterator(test_values + 1));
	REQUIRE(it == test_pointer_iterator(test_values + 0));

	REQUIRE(const_it == test_pointer_iterator(test_values + 0));
	REQUIRE(++const_it == test_pointer_iterator(test_values + 1));
	REQUIRE(const_it == test_pointer_iterator(test_values + 1));
	REQUIRE(const_it++ == test_pointer_iterator(test_values + 1));
	REQUIRE(const_it == test_pointer_iterator(test_values + 2));
	REQUIRE(--const_it == test_pointer_iterator(test_values + 1));
	REQUIRE(const_it == test_pointer_iterator(test_values + 1));
	REQUIRE(const_it-- == test_pointer_iterator(test_values + 1));
	REQUIRE(const_it == test_pointer_iterator(test_values + 0));
}

TEST_CASE_METHOD(test_fixture, "A number can be added to or subtracted from a pointer_iterator")
{
	test_pointer_iterator it(test_values);
	const_test_pointer_iterator const_it(test_values);

	REQUIRE((it += 1) == test_pointer_iterator(test_values + 1));
	REQUIRE((it += 2) == test_pointer_iterator(test_values + 3));
	REQUIRE((it += 3) == test_pointer_iterator(test_values + 6));
	REQUIRE((it += 4) == test_pointer_iterator(test_values + 10));
	REQUIRE((it -= 1) == test_pointer_iterator(test_values + 9));
	REQUIRE((it -= 2) == test_pointer_iterator(test_values + 7));
	REQUIRE((it -= 3) == test_pointer_iterator(test_values + 4));
	REQUIRE((it -= 4) == test_pointer_iterator(test_values + 0));

	REQUIRE((const_it += 1) == const_test_pointer_iterator(test_values + 1));
	REQUIRE((const_it += 2) == const_test_pointer_iterator(test_values + 3));
	REQUIRE((const_it += 3) == const_test_pointer_iterator(test_values + 6));
	REQUIRE((const_it += 4) == const_test_pointer_iterator(test_values + 10));
	REQUIRE((const_it -= 1) == const_test_pointer_iterator(test_values + 9));
	REQUIRE((const_it -= 2) == const_test_pointer_iterator(test_values + 7));
	REQUIRE((const_it -= 3) == const_test_pointer_iterator(test_values + 4));
	REQUIRE((const_it -= 4) == const_test_pointer_iterator(test_values + 0));

	for (std::size_t i = 0; i <= 10; ++i)
	{
		REQUIRE((it + i) == test_pointer_iterator(test_values + i));
		REQUIRE((const_it + i) == const_test_pointer_iterator(test_values + i));
		it += i;
		const_it += i;
		REQUIRE(it == test_pointer_iterator(test_values + i));
		REQUIRE(const_it == const_test_pointer_iterator(test_values + i));

		REQUIRE((it - i) == test_pointer_iterator(test_values));
		REQUIRE((const_it - i) == const_test_pointer_iterator(test_values));
		it -= i;
		const_it -= i;
		REQUIRE(it == test_pointer_iterator(test_values));
		REQUIRE(const_it == const_test_pointer_iterator(test_values));
	}
}

TEST_CASE_METHOD(test_fixture, "pointer_iterators can be subtracted to calculate the difference")
{
	for (test_pointer_iterator::difference_type i = -10; i <= 10; ++i)
	{
		REQUIRE((test_pointer_iterator(test_values + i) - test_pointer_iterator(test_values)) == i);
		REQUIRE((test_pointer_iterator(test_values) - test_pointer_iterator(test_values + i)) == -i);

		REQUIRE((test_pointer_iterator(test_values + i) - const_test_pointer_iterator(test_values)) == i);
		REQUIRE((test_pointer_iterator(test_values) - const_test_pointer_iterator(test_values + i)) == -i);

		REQUIRE((const_test_pointer_iterator(test_values + i) - test_pointer_iterator(test_values)) == i);
		REQUIRE((const_test_pointer_iterator(test_values) - test_pointer_iterator(test_values + i)) == -i);

		REQUIRE((const_test_pointer_iterator(test_values + i) - const_test_pointer_iterator(test_values)) == i);
		REQUIRE((const_test_pointer_iterator(test_values) - const_test_pointer_iterator(test_values + i)) == -i);
	}
}

TEST_CASE_METHOD(test_fixture, "pointer_iterator can be indexed")
{
	for (std::size_t i = 0; i < 10; ++i)
	{
		REQUIRE(test_pointer_iterator(test_values)[i].value == test_values[i].value);
		REQUIRE(const_test_pointer_iterator(test_values)[i].value == test_values[i].value);
	}
}

TEST_CASE_METHOD(test_fixture, "pointer_iterator can be ordered")
{
	REQUIRE_FALSE(test_pointer_iterator(test_values + 0) <  test_pointer_iterator(test_values + 0));
	REQUIRE_FALSE(test_pointer_iterator(test_values + 0) >  test_pointer_iterator(test_values + 0));
	REQUIRE      (test_pointer_iterator(test_values + 0) <= test_pointer_iterator(test_values + 0));
	REQUIRE      (test_pointer_iterator(test_values + 0) >= test_pointer_iterator(test_values + 0));

	REQUIRE_FALSE(test_pointer_iterator(test_values + 1) <  test_pointer_iterator(test_values + 0));
	REQUIRE      (test_pointer_iterator(test_values + 1) >  test_pointer_iterator(test_values + 0));
	REQUIRE_FALSE(test_pointer_iterator(test_values + 1) <= test_pointer_iterator(test_values + 0));
	REQUIRE      (test_pointer_iterator(test_values + 1) >= test_pointer_iterator(test_values + 0));

	REQUIRE      (test_pointer_iterator(test_values + 0) <  test_pointer_iterator(test_values + 1));
	REQUIRE_FALSE(test_pointer_iterator(test_values + 0) >  test_pointer_iterator(test_values + 1));
	REQUIRE      (test_pointer_iterator(test_values + 0) <= test_pointer_iterator(test_values + 1));
	REQUIRE_FALSE(test_pointer_iterator(test_values + 0) >= test_pointer_iterator(test_values + 1));

	REQUIRE_FALSE(test_pointer_iterator(test_values + 1) <  test_pointer_iterator(test_values + 1));
	REQUIRE_FALSE(test_pointer_iterator(test_values + 1) >  test_pointer_iterator(test_values + 1));
	REQUIRE      (test_pointer_iterator(test_values + 1) <= test_pointer_iterator(test_values + 1));
	REQUIRE      (test_pointer_iterator(test_values + 1) >= test_pointer_iterator(test_values + 1));

	
	REQUIRE_FALSE(test_pointer_iterator(test_values + 0) <  const_test_pointer_iterator(test_values + 0));
	REQUIRE_FALSE(test_pointer_iterator(test_values + 0) >  const_test_pointer_iterator(test_values + 0));
	REQUIRE      (test_pointer_iterator(test_values + 0) <= const_test_pointer_iterator(test_values + 0));
	REQUIRE      (test_pointer_iterator(test_values + 0) >= const_test_pointer_iterator(test_values + 0));

	REQUIRE_FALSE(test_pointer_iterator(test_values + 1) <  const_test_pointer_iterator(test_values + 0));
	REQUIRE      (test_pointer_iterator(test_values + 1) >  const_test_pointer_iterator(test_values + 0));
	REQUIRE_FALSE(test_pointer_iterator(test_values + 1) <= const_test_pointer_iterator(test_values + 0));
	REQUIRE      (test_pointer_iterator(test_values + 1) >= const_test_pointer_iterator(test_values + 0));

	REQUIRE      (test_pointer_iterator(test_values + 0) <  const_test_pointer_iterator(test_values + 1));
	REQUIRE_FALSE(test_pointer_iterator(test_values + 0) >  const_test_pointer_iterator(test_values + 1));
	REQUIRE      (test_pointer_iterator(test_values + 0) <= const_test_pointer_iterator(test_values + 1));
	REQUIRE_FALSE(test_pointer_iterator(test_values + 0) >= const_test_pointer_iterator(test_values + 1));

	REQUIRE_FALSE(test_pointer_iterator(test_values + 1) <  const_test_pointer_iterator(test_values + 1));
	REQUIRE_FALSE(test_pointer_iterator(test_values + 1) >  const_test_pointer_iterator(test_values + 1));
	REQUIRE      (test_pointer_iterator(test_values + 1) <= const_test_pointer_iterator(test_values + 1));
	REQUIRE      (test_pointer_iterator(test_values + 1) >= const_test_pointer_iterator(test_values + 1));

	
	REQUIRE_FALSE(const_test_pointer_iterator(test_values + 0) <  test_pointer_iterator(test_values + 0));
	REQUIRE_FALSE(const_test_pointer_iterator(test_values + 0) >  test_pointer_iterator(test_values + 0));
	REQUIRE      (const_test_pointer_iterator(test_values + 0) <= test_pointer_iterator(test_values + 0));
	REQUIRE      (const_test_pointer_iterator(test_values + 0) >= test_pointer_iterator(test_values + 0));

	REQUIRE_FALSE(const_test_pointer_iterator(test_values + 1) <  test_pointer_iterator(test_values + 0));
	REQUIRE      (const_test_pointer_iterator(test_values + 1) >  test_pointer_iterator(test_values + 0));
	REQUIRE_FALSE(const_test_pointer_iterator(test_values + 1) <= test_pointer_iterator(test_values + 0));
	REQUIRE      (const_test_pointer_iterator(test_values + 1) >= test_pointer_iterator(test_values + 0));

	REQUIRE      (const_test_pointer_iterator(test_values + 0) <  test_pointer_iterator(test_values + 1));
	REQUIRE_FALSE(const_test_pointer_iterator(test_values + 0) >  test_pointer_iterator(test_values + 1));
	REQUIRE      (const_test_pointer_iterator(test_values + 0) <= test_pointer_iterator(test_values + 1));
	REQUIRE_FALSE(const_test_pointer_iterator(test_values + 0) >= test_pointer_iterator(test_values + 1));

	REQUIRE_FALSE(const_test_pointer_iterator(test_values + 1) <  test_pointer_iterator(test_values + 1));
	REQUIRE_FALSE(const_test_pointer_iterator(test_values + 1) >  test_pointer_iterator(test_values + 1));
	REQUIRE      (const_test_pointer_iterator(test_values + 1) <= test_pointer_iterator(test_values + 1));
	REQUIRE      (const_test_pointer_iterator(test_values + 1) >= test_pointer_iterator(test_values + 1));

	
	REQUIRE_FALSE(const_test_pointer_iterator(test_values + 0) <  const_test_pointer_iterator(test_values + 0));
	REQUIRE_FALSE(const_test_pointer_iterator(test_values + 0) >  const_test_pointer_iterator(test_values + 0));
	REQUIRE      (const_test_pointer_iterator(test_values + 0) <= const_test_pointer_iterator(test_values + 0));
	REQUIRE      (const_test_pointer_iterator(test_values + 0) >= const_test_pointer_iterator(test_values + 0));

	REQUIRE_FALSE(const_test_pointer_iterator(test_values + 1) <  const_test_pointer_iterator(test_values + 0));
	REQUIRE      (const_test_pointer_iterator(test_values + 1) >  const_test_pointer_iterator(test_values + 0));
	REQUIRE_FALSE(const_test_pointer_iterator(test_values + 1) <= const_test_pointer_iterator(test_values + 0));
	REQUIRE      (const_test_pointer_iterator(test_values + 1) >= const_test_pointer_iterator(test_values + 0));

	REQUIRE      (const_test_pointer_iterator(test_values + 0) <  const_test_pointer_iterator(test_values + 1));
	REQUIRE_FALSE(const_test_pointer_iterator(test_values + 0) >  const_test_pointer_iterator(test_values + 1));
	REQUIRE      (const_test_pointer_iterator(test_values + 0) <= const_test_pointer_iterator(test_values + 1));
	REQUIRE_FALSE(const_test_pointer_iterator(test_values + 0) >= const_test_pointer_iterator(test_values + 1));

	REQUIRE_FALSE(const_test_pointer_iterator(test_values + 1) <  const_test_pointer_iterator(test_values + 1));
	REQUIRE_FALSE(const_test_pointer_iterator(test_values + 1) >  const_test_pointer_iterator(test_values + 1));
	REQUIRE      (const_test_pointer_iterator(test_values + 1) <= const_test_pointer_iterator(test_values + 1));
	REQUIRE      (const_test_pointer_iterator(test_values + 1) >= const_test_pointer_iterator(test_values + 1));
}

TEST_CASE_METHOD(test_fixture, "pointer_iterator can be reversed")
{
	std::reverse_iterator<test_pointer_iterator> it(test_pointer_iterator(test_values + 3));

	REQUIRE(it->value == 30);
	++it;
	REQUIRE(it->value == 20);
	++it;
	REQUIRE(it->value == 10);
}