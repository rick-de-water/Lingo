#ifndef H_LINGO_ARCHITECTURE
#define H_LINGO_ARCHITECTURE

// This header detects the processor architecture
// Generates a compile error when the architecture is not detected

// Processor types
#define LINGO_ARCHITECTURE_X86                   0x0001
#define LINGO_ARCHITECTURE_X64                   0x0002

// Endianness
#define LINGO_ARCHITECTURE_BIG_ENDIAN            0x0001
#define LINGO_ARCHITECTURE_LITTLE_ENDIAN         0x0002

// Detect architecture
#ifndef LINGO_ARCHITECTURE
	#if defined(__i386) || defined(__i386__) || defined(_M_IX86) || defined(_X86_)
		#define LINGO_ARCHITECTURE               LINGO_ARCHITECTURE_X86
	#elif defined(__x86_64) || defined(__x86_64__) || defined(__amd64) || defined(__amd64__) || defined(_M_AMD64) || defined(_M_X64)
		#define LINGO_ARCHITECTURE               LINGO_ARCHITECTURE_X64
	#endif
#endif

#if !defined(LINGO_ARCHITECTURE)
	#error                                        Unable to detect processor architecture
#endif

// Detect architecture bits
#ifndef LINGO_ARCHITECTURE_BITS
	#if LINGO_ARCHITECTURE == LINGO_ARCHITECTURE_X86
		#define LINGO_ARCHITECTURE_BITS          32
	#endif

	#if LINGO_ARCHITECTURE == LINGO_ARCHITECTURE_X64
		#define LINGO_ARCHITECTURE_BITS          64
	#endif
#endif

#ifndef LINGO_ARCHITECTURE_BYTES
	#if LINGO_ARCHITECTURE_BITS == 8
		#define LINGO_ARCHITECTURE_BYTES         1
	#elif LINGO_ARCHITECTURE_BITS == 16
		#define LINGO_ARCHITECTURE_BYTES         2
	#elif LINGO_ARCHITECTURE_BITS == 32
		#define LINGO_ARCHITECTURE_BYTES         4
	#elif LINGO_ARCHITECTURE_BITS == 64
		#define LINGO_ARCHITECTURE_BYTES         8
	#endif
#endif

#if !defined(LINGO_ARCHITECTURE_BITS) || !defined(LINGO_ARCHITECTURE_BYTES)
	#error                                        Unable to detect processor architecture bits
#endif

// Detect architecture endianness
#ifndef LINGO_ARCHITECTURE_ENDIANNESS
	#if LINGO_ARCHITECTURE == LINGO_ARCHITECTURE_X86 || LINGO_ARCHITECTURE == LINGO_ARCHITECTURE_X64
		#define LINGO_ARCHITECTURE_ENDIANNESS    LINGO_ARCHITECTURE_LITTLE_ENDIAN
	#endif
#endif

#if !defined(LINGO_ARCHITECTURE_ENDIANNESS)
	#error                                        Unable to detect processor architecture endianness
#endif

#endif