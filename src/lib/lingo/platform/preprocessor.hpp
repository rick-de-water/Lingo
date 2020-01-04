#ifndef H_LINGO_PLATFORM_PREPROCESSOR
#define H_LINGO_PLATFORM_PREPROCESSOR

#define LINGO_STRINGIFY(x) #x

#ifdef _MSC_VER
#define LINGO_PRAGMA(x) __pragma(x)
#else
#define LINGO_PRAGMA(x) _Pragma(LINGO_STRINGIFY(x))
#endif

#endif