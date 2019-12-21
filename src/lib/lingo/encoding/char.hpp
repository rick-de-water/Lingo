#ifndef H_LINGO_ENCODING_CHAR
#define H_LINGO_ENCODING_CHAR

#include <lingo/encoding/none.hpp>
#include <lingo/encoding/utf8.hpp>

namespace lingo
{
	namespace encoding
	{
		template <typename UnitT>
		struct char_default_encoding
		{
			using type = none<UnitT, UnitT>;
		};

		#if __cpp_char8_t
		template <>
		struct char_default_encoding<char8_t>
		{
			using type = utf8<char8_t, char32_t>;
		};
		#endif

		template <typename UnitT>
		using char_default_encoding_t = typename char_default_encoding<UnitT>::type;
	}
}

#endif