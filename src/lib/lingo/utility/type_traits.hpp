#ifndef H_LINGO_UTILITY_TYPE_TRAITS
#define H_LINGO_UTILITY_TYPE_TRAITS

#include <lingo/encoding/execution.hpp>
#include <lingo/page/execution.hpp>

#include <lingo/platform/constexpr.hpp>

#include <iterator>
#include <type_traits>

namespace lingo
{
	namespace utility
	{
		template <typename ...>
		using void_t = void;

		template <typename Encoding, typename = void>
		struct is_execution_encoding : std::false_type
		{
		};

		template <typename Encoding>
		struct is_execution_encoding<Encoding,
			typename std::enable_if<
				std::is_same<Encoding, lingo::encoding::execution_encoding_t<typename Encoding::unit_type>>::value>::type> : std::true_type
		{
		};

		#ifdef __cpp_variable_templates
		template <typename Encoding>
		LINGO_CONSTEXPR14 bool is_execution_encoding_v = is_execution_encoding<Encoding>::value;
		#endif

		template <typename Encoding, typename Page, typename = void>
		struct is_execution_set : std::false_type
		{
		};

		template <typename Encoding, typename Page>
		struct is_execution_set<Encoding, Page,
			typename std::enable_if<
				std::is_same<Encoding, lingo::encoding::execution_encoding_t<typename Encoding::unit_type>>::value &&
				std::is_same<Page, lingo::page::execution_page_t<typename Encoding::unit_type>>::value>::type> : std::true_type
		{
		};


		#ifdef __cpp_variable_templates
		template <typename Encoding, typename Page>
		LINGO_CONSTEXPR14 bool is_execution_set_v = is_execution_set<Encoding, Page>::value;
		#endif

		template <typename Iterator, typename Catagory, typename = void>
		struct is_catagory_iterator : std::false_type
		{
		};

		template <typename Iterator, typename Catagory>
		struct is_catagory_iterator<Iterator, Catagory,
			typename std::enable_if<std::is_same<typename std::iterator_traits<Iterator>::iterator_catagory, Catagory>::value>::type> : std::true_type
		{
		};

		#ifdef __cpp_variable_templates
		template <typename Iterator, typename Catagory>
		LINGO_CONSTEXPR14 bool is_catagory_iterator_v = is_catagory_iterator<Iterator, Catagory>::value;
		#endif

		template <typename Iterator>
		using is_input_iterator = is_catagory_iterator<Iterator, std::input_iterator_tag>;
		template <typename Iterator>
		using is_output_iterator = is_catagory_iterator<Iterator, std::output_iterator_tag>;
		template <typename Iterator>
		using is_forward_iterator = is_catagory_iterator<Iterator, std::forward_iterator_tag>;
		template <typename Iterator>
		using is_bidirectional_iterator = is_catagory_iterator<Iterator, std::bidirectional_iterator_tag>;
		template <typename Iterator>
		using is_random_access_iterator = is_catagory_iterator<Iterator, std::random_access_iterator_tag>;

		#ifdef __cpp_variable_templates
		template <typename Iterator>
		LINGO_CONSTEXPR14 bool is_input_iterator_v = is_input_iterator<Iterator>::value;
		template <typename Iterator>
		LINGO_CONSTEXPR14 bool is_output_iterator_v = is_output_iterator<Iterator>::value;
		template <typename Iterator>
		LINGO_CONSTEXPR14 bool is_forward_iterator_v = is_forward_iterator<Iterator>::value;
		template <typename Iterator>
		LINGO_CONSTEXPR14 bool is_bidirectional_iterator_v = is_bidirectional_iterator<Iterator>::value;
		template <typename Iterator>
		LINGO_CONSTEXPR14 bool is_random_access_iterator_v = is_random_access_iterator<Iterator>::value;
		#endif
	}
}

#endif