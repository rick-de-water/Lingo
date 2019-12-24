#ifndef H_LINGO_PAGE_ISO_8859_1
#define H_LINGO_PAGE_ISO_8859_1

#include <lingo/page/unicode.hpp>

#include <lingo/page/internal/iso_8859.hpp>
#include <lingo/page/internal/iso_8859_1_unicode_mapping.hpp>
#include <lingo/page/internal/unicode_iso_8859_1_mapping.hpp>

#include <type_traits>

namespace lingo
{
	namespace page
	{
		struct iso_8859_1 : public internal::iso_8859<
			internal::iso_8859_1_unicode_mapping<unicode::point_type>,
			internal::unicode_iso_8859_1_mapping<unicode::point_type>>
		{
		};
	}
}

#endif