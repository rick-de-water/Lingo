#ifndef H_LINGO_PLATFORM_ATTRIBUTES
#define H_LINGO_PLATFORM_ATTRIBUTES

#ifdef __has_cpp_attribute
	#if __has_cpp_attribute(maybe_unused)
		#define LINGO_ATTRIBUTE_MAYBE_UNUSED [[maybe_unused]]
	#endif
	#if __has_cpp_attribute(nodiscard)
		#define LINGO_ATTRIBUTE_NODISCARD [[nodiscard]]
	#endif
#endif



#ifndef LINGO_ATTRIBUTE_MAYBE_UNUSED
#define LINGO_ATTRIBUTE_MAYBE_UNUSED
#endif

#ifndef LINGO_ATTRIBUTE_NODISCARD
#define LINGO_ATTRIBUTE_NODISCARD
#endif

#endif