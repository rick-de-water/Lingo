#ifndef H_LINGO_ERROR_EXCEPTION
#define H_LINGO_ERROR_EXCEPTION

#include <lingo/error/error_code.hpp>

#include <stdexcept>

namespace lingo
{
	namespace error
	{
		class exception : public std::runtime_error
		{
			public:
			explicit exception(error::error_code error_code):
				runtime_error(error_to_string(error_code)),
				_error_code(error_code)
			{
			}

			error::error_code error_code() const noexcept
			{
				return _error_code;
			}

			private:
			error::error_code _error_code;

			static const char* error_to_string(error::error_code error_code)
			{
				switch (error_code)
				{
					case error_code::success:                      return "success";
					case error_code::source_buffer_too_small:      return "source buffer too small";
					case error_code::destination_buffer_too_small: return "destination buffer too small";
					case error_code::invalid_point:                return "invalid point";
					case error_code::invalid_unit:                 return "invalid unit";
					default:                                       return "unknown error code";
				}
			}
		};
	}
}

#endif