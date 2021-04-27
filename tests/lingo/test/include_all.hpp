#ifndef H_LINGO_TEST_INCLUDE_ALL
#define H_LINGO_TEST_INCLUDE_ALL

#if LINGO_TEST_SINGLE
#include <lingo/lingo.hpp>
#elif LINGO_TEST_MODULE
import lingo;
#else
#error "Unknown method of inclusion"
#endif

#endif