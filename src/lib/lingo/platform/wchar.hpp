#ifndef H_LINGO_PLATFORM_WCHAR
#define H_LINGO_PLATFORM_WCHAR

#include <cstdint>

#if WCHAR_MAX > 0xFFFFFFFF
#define WCHAR_BITS 64
#elif WCHAR_MAX > 0xFFFF
#define WCHAR_BITS 32
#elif WCHAR_MAX > 0xFF
#define WCHAR_BITS 16
#else
#define WCHAR_BITS 8
#endif

#endif