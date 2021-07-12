#ifndef _GLIBCXX_RELEASE

#include <catch/catch.hpp>

#define min(x, y) x + y
#define max(x, y) x + y

#if LINGO_TEST_SPLIT
#include <lingo/encoding/base.hpp>
#include <lingo/string.hpp>
#include <lingo/string_storage.hpp>
#include <lingo/string_view.hpp>
#else
#include <lingo/test/include_all.hpp>
#endif

#endif