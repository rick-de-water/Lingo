#ifndef H_LINGO_CONSTEXPR
#define H_LINGO_CONSTEXPR

#include <lingo/platform/attributes.hpp>

#ifdef __cpp_constexpr
#define LINGO_HAS_CONSTEXPR11
#if __cpp_constexpr >= 201304
#define LINGO_HAS_CONSTEXPR14
#if __cpp_constexpr >= 201603
#define LINGO_HAS_CONSTEXPR17
#if __cpp_constexpr >= 201907
#define LINGO_HAS_CONSTEXPR20
#endif
#endif
#endif
#endif

#ifdef __cpp_if_constexpr
#define LINGO_HAS_IF_CONSTEXPR
#endif

#ifdef LINGO_HAS_CONSTEXPR11
#define LINGO_CONSTEXPR11 constexpr
#else
#define LINGO_CONSTEXPR11
#endif

#ifdef LINGO_HAS_CONSTEXPR14
#define LINGO_CONSTEXPR14 constexpr
#else
#define LINGO_CONSTEXPR14
#endif

#ifdef LINGO_HAS_CONSTEXPR17
#define LINGO_CONSTEXPR17 constexpr
#else
#define LINGO_CONSTEXPR17
#endif

#ifdef LINGO_HAS_CONSTEXPR20
#define LINGO_CONSTEXPR20 constexpr
#else
#define LINGO_CONSTEXPR20
#endif

#ifdef LINGO_HAS_IF_CONSTEXPR
#ifdef _MSC_VER
#define LINGO_IF_CONSTEXPR __pragma(warning(suppress: 4127)) if constexpr
#else
#define LINGO_IF_CONSTEXPR if constexpr
#endif
#else
#ifdef _MSC_VER
#define LINGO_IF_CONSTEXPR __pragma(warning(suppress: 4127)) if
#else
#define LINGO_IF_CONSTEXPR if
#endif
#endif

#ifdef LINGO_HAS_IF_CONSTEXPR
#define LINGO_UNUSED_IF_CONSTEXPR(x) LINGO_ATTRIBUTE_MAYBE_UNUSED x
#else
#define LINGO_UNUSED_IF_CONSTEXPR(x) x
#endif

#endif