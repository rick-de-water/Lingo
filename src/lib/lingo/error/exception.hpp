#ifndef H_LINGO_ERROR_EXCEPTION
#define H_LINGO_ERROR_EXCEPTION

#include <lingo/error/error_code.hpp>

#include <stdexcept>

namespace lingo
{
	namespace error
	{
		class exception : std::runtime_error
		{
			public:
			explicit exception(error::error_code error_code):
				_error_code(error_code),
				runtime_error(error_to_string(error_code))
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
					case error_code::success:                 "success";
					case error_code::buffer_too_small:        "buffer too small";
					case error_code::invalid_point:           "invalid point";
					case error_code::invalid_first_unit:      "invalid first unit";
					case error_code::invalid_subsequent_unit: "invalid subsequent unit";
					default: return                           "invalid error code";
				}
			}
		};
	}
}

#endif