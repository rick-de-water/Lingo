#ifndef H_LINGO_PLATFORM_COMPILER
#define H_LINGO_PLATFORM_COMPILER

// This header detects the compiler type
// Generates a compile error when the compiler is not detected

#include <lingo/platform/preprocessor.hpp>

// Compiler types
#define LINGO_COMPILER_UNKNOWN                     0x0000
#define LINGO_COMPILER_MSVC                        0x0001
#define LINGO_COMPILER_CLANG                       0x0002
#define LINGO_COMPILER_INTEL                       0x0003
#define LINGO_COMPILER_GCC                         0x0004

#ifndef LINGO_COMPILER
	// Visual studio
	#if defined(_MSC_VER)
		#define LINGO_COMPILER                     LINGO_COMPILER_MSVC
		#define LINGO_COMPILER_VERSION_MAJOR       (_MSC_VER / 100)
		#define LINGO_COMPILER_VERSION_MINOR       (_MSC_VER - (LINGO_COMPILER_VERSION_MAJOR * 100))
		#if defined(_MSC_BUILD)
			#define LINGO_COMPILER_VERSION_PATCH   _MSC_BUILD
		#else
			#define LINGO_COMPILER_VERSION_PATCH		0
		#endif
		#if defined(_MSC_FULL_VER)
			#define LINGO_COMPILER_VERSION_STRING	"MSVC " LINGO_STRINGIFY(_MSC_FULL_VER)
		#else
			#define LINGO_COMPILER_VERSION_STRING	"MSVC " LINGO_STRINGIFY(_MSC_VER)
		#endif

	// Clang
	#elif defined(__clang__)
		#define LINGO_COMPILER                     LINGO_COMPILER_CLANG
		#define LINGO_COMPILER_VERSION_MAJOR       __clang_major__
		#define LINGO_COMPILER_VERSION_MINOR       __clang_minor__
		#define LINGO_COMPILER_VERSION_PATCH       __clang_patchlevel__
		#define LINGO_COMPILER_VERSION_STRING      __VERSION__ 

	// Intel
	#elif defined(__ICC) || defined (__INTEL_COMPILER)
		#define LINGO_COMPILER                     LINGO_COMPILER_INTEL
		#define LINGO_COMPILER_VERSION_MAJOR       __GNUG__
		#define LINGO_COMPILER_VERSION_MINOR       __GNUC_MINOR__
		#define LINGO_COMPILER_VERSION_PATCH       __GNUC_PATCHLEVEL__
		#define LINGO_COMPILER_VERSION_STRING      __VERSION__ 
		
	// GCC
	#elif (defined(__GNUC__) || defined(__GNUG__))
		#define LINGO_COMPILER                     LINGO_COMPILER_GCC
		#define LINGO_COMPILER_VERSION_MAJOR       __GNUG__
		#define LINGO_COMPILER_VERSION_MINOR       __GNUC_MINOR__
		#if defined(__GNUC_PATCHLEVEL__)
			#define LINGO_COMPILER_VERSION_PATCH   __GNUC_PATCHLEVEL__
		#else
			#define LINGO_COMPILER_VERSION_PATCH   0
		#endif
		#define LINGO_COMPILER_VERSION_STRING      __VERSION__

	#endif
#endif

// Unable to detect the compiler
#ifndef LINGO_COMPILER
	#error                                         Unable to detect compiler
#endif

#define LINGO_COMPILER_VERSION                     ((LINGO_COMPILER_VERSION_MAJOR << 16) | (LINGO_COMPILER_VERSION_MINOR << 8) | (LINGO_COMPILER_VERSION_PATCH))

#ifndef LINGO_COMPILER_VERSION_STRING
	#define LINGO_COMPILER_VERSION_STRING          LINGO_STRINGIFY(LINGO_COMPILER_VERSION_MAJOR) "." LINGO_STRINGIFY(LINGO_COMPILER_VERSION_MINOR) "." LINGO_STRINGIFY(LINGO_COMPILER_VERSION_PATCH)
#endif

#endif