#ifndef H_LINGO_PAGE_ISO_8859_2
#define H_LINGO_PAGE_ISO_8859_2

#include <lingo/page/unicode.hpp>

#include <lingo/page/internal/iso_8859.hpp>
#include <lingo/page/internal/iso_8859_2_unicode_mapping.hpp>
#include <lingo/page/internal/unicode_iso_8859_2_mapping.hpp>

#include <type_traits>

namespace lingo
{
	namespace page
	{
		struct iso_8859_2 : public internal::iso_8859<
			internal::iso_8859_2_unicode_mapping<unicode::point_type>,
			internal::unicode_iso_8859_2_mapping<unicode::point_type>>
		{
		};
	}
}

#endif