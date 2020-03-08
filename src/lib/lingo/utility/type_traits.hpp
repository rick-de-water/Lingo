#ifndef H_LINGO_UTILITY_TYPE_TRAITS
#define H_LINGO_UTILITY_TYPE_TRAITS

#include <lingo/encoding/execution.hpp>
#include <lingo/page/execution.hpp>

#include <lingo/platform/constexpr.hpp>

#include <iterator>
#include <memory>
#include <type_traits>

namespace lingo
{
	namespace internal
	{
		template <typename Encoding>
		using default_allocator = std::allocator<typename Encoding::unit_type>;
	}

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
		LINGO_CONSTEXPR14 const bool is_execution_encoding_v = is_execution_encoding<Encoding>::value;
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
		LINGO_CONSTEXPR14 const bool is_execution_set_v = is_execution_set<Encoding, Page>::value;
		#endif


		template <typename Encoding1, typename Encoding2>
		struct is_same_encoding : std::false_type
		{
		};

		template <template <typename...> class Encoding, typename... Ts1, typename... Ts2>
		struct is_same_encoding<Encoding<Ts1...>, Encoding<Ts2...>> : std::true_type
		{
		};

		#ifdef __cpp_variable_templates
		template <typename Encoding1, typename Encoding2>
		LINGO_CONSTEXPR14 const bool is_same_encoding_v = is_same_encoding<Encoding1, Encoding2>::value;
		#endif

		template <typename Encoding, typename Page, typename = void>
		struct is_char_compatible : std::false_type
		{
		};

		template <typename Encoding, typename Page>
		struct is_char_compatible<Encoding, Page,
			typename std::enable_if<
				!std::is_same<typename Encoding::unit_type, char>::value &&
				(sizeof(typename Encoding::unit_type) == sizeof(char)) &&
				utility::is_same_encoding<Encoding, typename encoding::execution_encoding<char>::type>::value &&
				std::is_same<Page, typename page::execution_page<char>::type>::value>::type> : std::true_type
		{
		};

		#ifdef __cpp_variable_templates
		template <typename Encoding, typename Page>
		LINGO_CONSTEXPR14 const bool is_char_compatible_v = is_char_compatible<Encoding, Page>::value;
		#endif

		template <typename Iterator, typename Catagory, typename = void>
		struct is_catagory_iterator : std::false_type
		{
		};

		template <typename Iterator, typename Catagory>
		struct is_catagory_iterator<Iterator, Catagory,
			typename std::enable_if<std::is_base_of<Catagory, typename std::iterator_traits<Iterator>::iterator_catagory>::value>::type> : std::true_type
		{
		};

		#ifdef __cpp_variable_templates
		template <typename Iterator, typename Catagory>
		LINGO_CONSTEXPR14 const bool is_catagory_iterator_v = is_catagory_iterator<Iterator, Catagory>::value;
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
		#ifdef __cpp_lib_ranges
		template <typename Iterator>
		using is_contiguous_iterator = is_catagory_iterator<Iterator, std::contiguous_iterator_tag>;
		#endif

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
		#ifdef __cpp_lib_ranges
		template <typename Iterator>
		LINGO_CONSTEXPR14 bool is_contiguous_iterator_v = is_contiguous_iterator<Iterator>::value;
		#endif
		#endif

		template <typename StringObject>
		struct string_traits
		{
			using encoding_type = typename StringObject::encoding_type;
			using page_type = typename StringObject::page_type;

			using unit_type = typename encoding_type::unit_type;
			using point_type = typename page_type::point_type;
		};

		template <typename T>
		struct string_traits<T*>
		{
			using unit_type = typename std::remove_cv<T>::type;
			using encoding_type = lingo::encoding::execution_encoding_t<unit_type>;
			using page_type = lingo::page::execution_page_t<unit_type>;
			using point_type = typename page_type::point_type;
		};
	}
}

#endif