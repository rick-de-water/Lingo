#ifndef H_LINGO_PLATFORM_PRAGMA
#define H_LINGO_PLATFORM_PRAGMA

#include <lingo/platform/compiler.hpp>
#include <lingo/platform/preprocessor.hpp>

#if LINGO_COMPILER == LINGO_COMPILER_MSVC
#define LINGO_PRAGMA(x) __pragma(x)
#else
#define LINGO_PRAGMA(x) _Pragma(LINGO_STRINGIFY(x))
#endif

#endif