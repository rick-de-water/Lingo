#ifndef H_LINGO_ERROR_ERROR_CODE
#define H_LINGO_ERROR_ERROR_CODE

namespace lingo
{
	namespace error
	{
		enum class error_code
		{
			none,

			buffer_too_small,
			invalid_point,
			invalid_first_unit,
			invalid_subsequent_unit,
		};
	}
}

#endif