#ifndef H_LINGO_PAGE_UNICODE_VERSION
#define H_LINGO_PAGE_UNICODE_VERSION

namespace lingo
{
	namespace page
	{
		enum class unicode_version
		{
			none = 0,

			v1_1  = 0x00010100,
			v2_0  = 0x00020000,
			v2_1  = 0x00020100,
			v3_0  = 0x00030000,
			v3_1  = 0x00030100,
			v3_2  = 0x00030200,
			v4_0  = 0x00040000,
			v4_1  = 0x00040100,
			v5_0  = 0x00050000,
			v5_1  = 0x00050100,
			v5_2  = 0x00050200,
			v6_0  = 0x00060000,
			v6_1  = 0x00060100,
			v6_2  = 0x00060200,
			v6_3  = 0x00060300,
			v7_0  = 0x00070000,
			v8_0  = 0x00080000,
			v9_0  = 0x00090000,
			v10_0 = 0x00100000,
			v11_0 = 0x00110000,
			v12_0 = 0x00120000,
			v12_1 = 0x00120100,
		};
	}
}

#endif