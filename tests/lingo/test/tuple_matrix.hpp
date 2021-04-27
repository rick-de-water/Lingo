#ifndef H_LINGO_TEST_TUPLE_MATRIX
#define H_LINGO_TEST_TUPLE_MATRIX

#include <lingo/utility/integer_sequence.hpp>

#include <cstddef>
#include <tuple>
#include <utility>

namespace lingo
{
	namespace test
	{
		// Concat tuples
		template <typename IndexSequence1, typename IndexSequence2>
		struct tuple_concat;

		template <typename... Types1, typename... Types2>
		struct tuple_concat<std::tuple<Types1...>, std::tuple<Types2...>>
		{
			using type = std::tuple<Types1..., Types2...>;
		};

		template <typename Types1, typename Types2>
		using tuple_concat_t = typename tuple_concat<Types1, Types2>::type;


		// Combine tuples
		template <typename IndexSequence, typename... Tuples>
		struct tuple_combine;

		template <typename FirstTuple, std::size_t FirstIndex, typename... Tuples, std::size_t... Indices>
		struct tuple_combine<utility::index_sequence<FirstIndex, Indices...>, FirstTuple, Tuples...>
		{
			private:
			using current_type = typename std::remove_reference<decltype(std::get<FirstIndex>(std::declval<FirstTuple>()))>::type;
			using current_tuple = std::tuple<current_type>;
			using next_tuple = typename tuple_combine<utility::index_sequence<Indices...>, Tuples...>::type;
			using combined_tuple = typename tuple_concat<current_tuple, next_tuple>::type;

			public:
			using type = combined_tuple;
		};

		template <typename FirstTuple, std::size_t FirstIndex>
		struct tuple_combine<utility::index_sequence<FirstIndex>, FirstTuple>
		{
			private:
			using current_type = typename std::remove_reference<decltype(std::get<FirstIndex>(std::declval<FirstTuple>()))>::type;
			using current_tuple = std::tuple<current_type>;

			public:
			using type = current_tuple;
		};

		template <typename IndexSequence, typename... Tuples>
		using tuple_combine_t = typename tuple_combine<IndexSequence, Tuples...>::type;


		// Next index sequence
		template <typename IndexSequence, typename... Tuples>
		struct next_index_sequence;

		template <typename FirstTuple, std::size_t FirstIndex, typename... Tuples, std::size_t... Is>
		struct next_index_sequence<utility::index_sequence<FirstIndex, Is...>, FirstTuple, Tuples...>
		{
			using child = next_index_sequence<utility::index_sequence<Is...>, Tuples...>;

			static constexpr bool carry = child::carry && ((FirstIndex + 1) == std::tuple_size<FirstTuple>::value);
			static constexpr std::size_t value = child::carry ? (carry ? 0 : FirstIndex + 1) : FirstIndex;
			using type = utility::index_sequence_concat_t<utility::index_sequence<value>, typename child::type>;
		};

		template <typename FirstTuple, std::size_t FirstIndex>
		struct next_index_sequence<utility::index_sequence<FirstIndex>, FirstTuple>
		{
			static constexpr bool carry = (FirstIndex + 1) == std::tuple_size<FirstTuple>::value;
			static constexpr std::size_t value = carry ? 0 : FirstIndex + 1;
			using type = utility::index_sequence<value>;
		};

		template <typename IndexSequence, typename... Tuples>
		using next_index_sequence_t = typename next_index_sequence<IndexSequence, Tuples...>::type;


		// End index sequence
		template <std::size_t FirstIndex, typename IndexSequence>
		struct end_index_sequence_generator;

		template <std::size_t FirstIndex, std::size_t... Is>
		struct end_index_sequence_generator<FirstIndex, utility::index_sequence<Is...>>
		{
			using type = utility::index_sequence<FirstIndex, Is...>;
		};

		template <typename FirstTuple, typename... Tuples>
		struct end_index_sequence
		{
			using type = typename end_index_sequence_generator<std::tuple_size<FirstTuple>::value, utility::make_zero_index_sequence_t<sizeof...(Tuples)>>::type;
		};

		template <typename FirstTuple, typename... Tuples>
		using end_index_sequence_t = typename end_index_sequence<FirstTuple, Tuples...>::type;


		// Tuple matrix
		template <typename IndexSequence, bool End, typename... Tuples>
		struct tuple_matrix_impl;

		template <typename IndexSequence, typename... Tuples>
		struct tuple_matrix_impl<IndexSequence, false, Tuples...>
		{
			private:
			using current_tuple = typename tuple_combine<IndexSequence, Tuples...>::type;
			using next_sequence = next_index_sequence<IndexSequence, Tuples...>;
			using next_tuples = typename tuple_matrix_impl<typename next_sequence::type, next_sequence::carry, Tuples...>::type;

			public:
			using type = tuple_concat_t<std::tuple<current_tuple>, next_tuples>;
		};

		template <typename IndexSequence, typename... Tuples>
		struct tuple_matrix_impl<IndexSequence, true, Tuples...>
		{
			using type = std::tuple<>;
		};

		template <typename... Tuples>
		using tuple_matrix = tuple_matrix_impl<utility::make_zero_index_sequence_t<sizeof...(Tuples)>, next_index_sequence<utility::make_zero_index_sequence_t<sizeof...(Tuples)>, Tuples...>::carry, Tuples...>;

		template <typename... Tuples>
		using tuple_matrix_t = typename tuple_matrix<Tuples...>::type;
	}
}

#endif