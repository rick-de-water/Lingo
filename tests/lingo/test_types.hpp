#ifndef H_LINGO_TEST_TYPES
#define H_LINGO_TEST_TYPES

#include <cstdint>
#include <cstddef>
#include <tuple>
#include <utility>

namespace lingo
{
	namespace test
	{
		using unit_types = std::tuple<
			int_least8_t,
			int_least16_t,
			int_least32_t,
			int_least64_t

			/*#ifndef _DEBUG
			,
			char,
			signed char,
			unsigned char,
			wchar_t,
			#if __cpp_char8_t
			char8_t,
			#endif
			char16_t,
			char32_t,
			uint_least8_t,
			uint_least16_t,
			uint_least32_t,
			uint_least64_t
			#endif*/
		>;

		using point_types = unit_types;

		template <std::size_t... Is>
		constexpr auto zero_index_sequence_generator(const std::index_sequence<Is...>&)
		{
			return std::index_sequence<(Is, 0)...>();
		}

		template <std::size_t N>
		using zero_index_sequence = decltype(zero_index_sequence_generator(std::declval<std::make_index_sequence<N>>()));

		// Concat index sequence
		template <typename IndexSequence1, typename IndexSequence2>
		struct index_sequence_concat;

		template <std::size_t... Indices1, std::size_t... Indices2>
		struct index_sequence_concat<std::index_sequence<Indices1...>, std::index_sequence<Indices2...>>
		{
			using type = std::index_sequence<Indices1..., Indices2...>;
		};

		template <typename IndexSequence1, typename IndexSequence2>
		using index_sequence_concat_t = typename index_sequence_concat<IndexSequence1, IndexSequence2>::type;


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

		/*template <typename Tuple, std::size_t Index, typename... Tuples, std::size_t... Is, std::enable_if_t<(sizeof...(Tuples) > 0), int> = 0>
		constexpr auto combined_tuple(const std::tuple<Tuple, Tuples...>&, const std::index_sequence<Index, Is...>&)
		{
			using current_type = typename std::remove_reference<decltype(std::get<Index>(std::declval<Tuple>()))>::type;
			using current_tuple = std::tuple<current_type>;
			using next_tuple = decltype(combined_tuple(std::declval<std::tuple<Tuples...>>(), std::declval<std::index_sequence<Is...>>()));
			using combined_type = decltype(std::tuple_cat(std::declval<current_tuple>(), std::declval<next_tuple>()));

			return combined_type();
		}

		template <typename Tuple, std::size_t Index, typename... Tuples, std::size_t... Is, std::enable_if_t<(sizeof...(Tuples) == 0), int> = 0>
		constexpr auto combined_tuple(const std::tuple<Tuple, Tuples...>&, const std::index_sequence<Index, Is...>&)
		{
			using current_type = typename std::remove_reference<decltype(std::get<Index>(std::declval<Tuple>()))>::type;
			using current_tuple = std::tuple<current_type>;

			return current_tuple();
		}*/

		template <typename IndexSequence, typename... Tuples>
		struct tuple_combine;

		template <typename FirstTuple, std::size_t FirstIndex, typename... Tuples, std::size_t... Indices>
		struct tuple_combine<std::index_sequence<FirstIndex, Indices...>, FirstTuple, Tuples...>
		{
			private:
			using current_type = typename std::remove_reference<decltype(std::get<FirstIndex>(std::declval<FirstTuple>()))>::type;
			using current_tuple = std::tuple<current_type>;
			using next_tuple = typename tuple_combine<std::index_sequence<Indices...>, Tuples...>::type;
			using combined_tuple = typename tuple_concat<current_tuple, next_tuple>::type;

			public:
			using type = combined_tuple;
		};

		template <typename FirstTuple, std::size_t FirstIndex>
		struct tuple_combine<std::index_sequence<FirstIndex>, FirstTuple>
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
		struct next_index_sequence<std::index_sequence<FirstIndex, Is...>, FirstTuple, Tuples...>
		{
			using child = next_index_sequence<std::index_sequence<Is...>, Tuples...>;

			static constexpr bool carry = child::carry && ((FirstIndex + 1) == std::tuple_size<FirstTuple>::value);
			static constexpr std::size_t value = child::carry ? (carry ? 0 : FirstIndex + 1) : FirstIndex;
			using type = index_sequence_concat_t<std::index_sequence<value>, typename child::type>;
		};

		template <typename FirstTuple, std::size_t FirstIndex>
		struct next_index_sequence<std::index_sequence<FirstIndex>, FirstTuple>
		{
			static constexpr bool carry = (FirstIndex + 1) == std::tuple_size<FirstTuple>::value;
			static constexpr std::size_t value = carry ? 0 : FirstIndex + 1;
			using type = std::index_sequence<value>;
		};

		template <typename IndexSequence, typename... Tuples>
		using next_index_sequence_t = typename next_index_sequence<IndexSequence, Tuples...>::type;


		// End index sequence
		template <std::size_t FirstIndex, typename IndexSequence>
		struct end_index_sequence_generator;

		template <std::size_t FirstIndex, std::size_t... Is>
		struct end_index_sequence_generator<FirstIndex, std::index_sequence<Is...>>
		{
			using type = std::index_sequence<FirstIndex, Is...>;
		};

		template <typename FirstTuple, typename... Tuples>
		struct end_index_sequence
		{
			using type = typename end_index_sequence_generator<std::tuple_size<FirstTuple>::value, zero_index_sequence<sizeof...(Tuples)>>::type;
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
		using tuple_matrix = tuple_matrix_impl<zero_index_sequence<sizeof...(Tuples)>, next_index_sequence<zero_index_sequence<sizeof...(Tuples)>, Tuples...>::carry, Tuples...>;

		template <typename... Tuples>
		using tuple_matrix_t = typename tuple_matrix<Tuples...>::type;
	}
}

#endif