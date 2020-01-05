#ifndef H_LINGO_UTILITY_INTEGER_SEQUENCE
#define H_LINGO_UTILITY_INTEGER_SEQUENCE

#include <cstddef>
#include <type_traits>

namespace lingo
{
	namespace utility
	{
		// integer_sequence is a C++14 feature, so we make our own!
		template <typename T, T... Is>
		struct integer_sequence {};

		template <std::size_t... Is>
		using index_sequence = integer_sequence<std::size_t, Is...>;

		// Incrementing integer sequence
		namespace internal
		{
			template <typename T, typename N, typename... Is>
			struct incrementing_integer_sequence_generator : public incrementing_integer_sequence_generator<T, std::integral_constant<T, N::value - 1>, std::integral_constant<T, N::value - 1>, Is...> {};

			template <typename T, typename... Is>
			struct incrementing_integer_sequence_generator<T, std::integral_constant<T, 0>, Is...>
			{
				using type = integer_sequence<T, Is::value...>;
			};
		}

		template <typename T, T N>
		using make_incrementing_integer_sequence = internal::incrementing_integer_sequence_generator<T, std::integral_constant<T, N>>;
		template <std::size_t N>
		using make_incrementing_index_sequence = make_incrementing_integer_sequence<std::size_t, N>;


		template <typename T, T N>
		using make_incrementing_integer_sequence_t = typename make_incrementing_integer_sequence<T, N>::type;
		template <std::size_t N>
		using make_incrementing_index_sequence_t = typename make_incrementing_index_sequence<N>::type;

		static_assert(std::is_same<make_incrementing_index_sequence_t<10>, index_sequence<0, 1, 2, 3, 4, 5, 6, 7, 8, 9>>::value, "make_incrementing_index_sequence generates the correct sequence");

		// Repeating integer sequence
		namespace internal
		{
			template <typename T, typename N, typename X, typename... Is>
			struct repeating_integer_sequence_generator : public repeating_integer_sequence_generator<T, std::integral_constant<T, N::value - 1>, X, X, Is...> {};

			template <typename T, typename X, typename... Is>
			struct repeating_integer_sequence_generator<T, std::integral_constant<T, 0>, X, Is...>
			{
				using type = integer_sequence<T, Is::value...>;
			};
		}

		template <typename T, T N, T X>
		using make_repeating_integer_sequence = internal::repeating_integer_sequence_generator<T, std::integral_constant<T, N>, std::integral_constant<T, X>>;
		template <std::size_t N, std::size_t X>
		using make_repeating_index_sequence = make_repeating_integer_sequence<std::size_t, N, X>;

		template <typename T, T N, T X>
		using make_repeating_integer_sequence_t = typename make_repeating_integer_sequence<T, N, X>::type;
		template <std::size_t N, std::size_t X>
		using make_repeating_index_sequence_t = typename make_repeating_index_sequence<N, X>::type;

		static_assert(std::is_same<make_repeating_index_sequence_t<8, 4>, index_sequence<4, 4, 4, 4, 4, 4, 4, 4>>::value, "make_repeating_index_sequence generates the correct sequence");

		// Repeating zero sequence
		template <typename T, T N>
		using make_zero_integer_sequence = make_repeating_integer_sequence<T, N, 0>;
		template <std::size_t N>
		using make_zero_index_sequence = make_zero_integer_sequence<std::size_t, N>;

		template <typename T, T N>
		using make_zero_integer_sequence_t = typename make_zero_integer_sequence<T, N>::type;
		template <std::size_t N>
		using make_zero_index_sequence_t = typename make_zero_index_sequence<N>::type;

		static_assert(std::is_same<make_zero_index_sequence_t<5>, index_sequence<0, 0, 0, 0, 0>>::value, "make_zero_index_sequence generates the correct sequence");

		// Concat index sequence
		template <typename IndexSequence1, typename IndexSequence2>
		struct index_sequence_concat;

		template <std::size_t... Indices1, std::size_t... Indices2>
		struct index_sequence_concat<index_sequence<Indices1...>, index_sequence<Indices2...>>
		{
			using type = index_sequence<Indices1..., Indices2...>;
		};

		template <typename IndexSequence1, typename IndexSequence2>
		using index_sequence_concat_t = typename index_sequence_concat<IndexSequence1, IndexSequence2>::type;

	}
}

#endif