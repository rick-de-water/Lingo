#ifndef H_LINGO_PLATFORM_ENDIAN
#define H_LINGO_PLATFORM_ENDIAN

#include <lingo/constexpr.hpp>

#include <lingo/platform/architecture.hpp>

#include <climits>
#include <type_traits>

namespace lingo
{
	namespace platform
	{
		namespace internal
		{
			template <typename T>
			struct endian_constants
			{
				static LINGO_CONSTEXPR11 T char_bit_count = CHAR_BIT;
				static LINGO_CONSTEXPR11 T char_bit_mask = static_cast<T>(static_cast<unsigned char>(~0));
			};

			template <typename T>
			T common_swap_endian(T value) noexcept
			{
				unsigned char* ptr = reinterpret_cast<unsigned char*>(&value);

				for (size_t i = 0; i < (sizeof(T) / 2); ++i)
				{
					const unsigned char tmp = ptr[i];
					const size_t index = sizeof(T) - (i + 1);

					ptr[i] = ptr[index];
					ptr[index] = tmp;
				}

				return value;
			}
		}

		LINGO_CONSTEXPR11 bool is_big_endian() noexcept
		{
			return LINGO_ARCHITECTURE_ENDIANNESS == LINGO_ARCHITECTURE_BIG_ENDIAN;
		}

		LINGO_CONSTEXPR11 bool is_little_endian() noexcept
		{
			return LINGO_ARCHITECTURE_ENDIANNESS == LINGO_ARCHITECTURE_LITTLE_ENDIAN;
		}

		template <typename T>
		typename std::enable_if<!std::is_integral<T>::value, T>::type
			swap_endian(T value) noexcept
		{
			return internal::common_swap_endian(value);
		}

		template <typename T>
		LINGO_CONSTEXPR11 typename std::enable_if<
			std::is_integral<T>::value &&
			sizeof(T) == 1, T>::type
			swap_endian(T value) noexcept
		{
			return value;
		}

		template <typename T>
		LINGO_CONSTEXPR11 typename std::enable_if<
			std::is_integral<T>::value &&
			std::is_unsigned<T>::value &&
			sizeof(T) == 2, T>::type
			swap_endian(T value) noexcept
		{
			return
				(value << internal::endian_constants<T>::char_bit_count) |
				(value >> internal::endian_constants<T>::char_bit_count);
		}

		template <typename T>
		LINGO_CONSTEXPR11 typename std::enable_if<
			std::is_integral<T>::value &&
			std::is_unsigned<T>::value &&
			sizeof(T) == 4, T>::type
			swap_endian(T value) noexcept
		{
			return
				((value & (internal::endian_constants<T>::char_bit_mask << (internal::endian_constants<T>::char_bit_count * 0))) << internal::endian_constants<T>::char_bit_count * 3) |
				((value & (internal::endian_constants<T>::char_bit_mask << (internal::endian_constants<T>::char_bit_count * 1))) << internal::endian_constants<T>::char_bit_count * 1) |
				((value & (internal::endian_constants<T>::char_bit_mask << (internal::endian_constants<T>::char_bit_count * 2))) >> internal::endian_constants<T>::char_bit_count * 1) |
				((value & (internal::endian_constants<T>::char_bit_mask << (internal::endian_constants<T>::char_bit_count * 3))) >> internal::endian_constants<T>::char_bit_count * 3);
		}

		template <typename T>
		LINGO_CONSTEXPR11 typename std::enable_if<
			std::is_integral<T>::value &&
			std::is_unsigned<T>::value &&
			sizeof(T) == 8, T>::type
			swap_endian(T value) noexcept
		{
			return
				((value & (internal::endian_constants<T>::char_bit_mask << (internal::endian_constants<T>::char_bit_count * 0))) << internal::endian_constants<T>::char_bit_count * 7) |
				((value & (internal::endian_constants<T>::char_bit_mask << (internal::endian_constants<T>::char_bit_count * 1))) << internal::endian_constants<T>::char_bit_count * 5) |
				((value & (internal::endian_constants<T>::char_bit_mask << (internal::endian_constants<T>::char_bit_count * 2))) << internal::endian_constants<T>::char_bit_count * 3) |
				((value & (internal::endian_constants<T>::char_bit_mask << (internal::endian_constants<T>::char_bit_count * 3))) << internal::endian_constants<T>::char_bit_count * 1) |
				((value & (internal::endian_constants<T>::char_bit_mask << (internal::endian_constants<T>::char_bit_count * 4))) >> internal::endian_constants<T>::char_bit_count * 1) |
				((value & (internal::endian_constants<T>::char_bit_mask << (internal::endian_constants<T>::char_bit_count * 5))) >> internal::endian_constants<T>::char_bit_count * 3) |
				((value & (internal::endian_constants<T>::char_bit_mask << (internal::endian_constants<T>::char_bit_count * 6))) >> internal::endian_constants<T>::char_bit_count * 5) |
				((value & (internal::endian_constants<T>::char_bit_mask << (internal::endian_constants<T>::char_bit_count * 7))) >> internal::endian_constants<T>::char_bit_count * 7);
		}

		template <typename T>
		LINGO_CONSTEXPR11 typename std::enable_if<
			std::is_integral<T>::value &&
			std::is_signed<T>::value &&
			sizeof(T) == 2, T>::type
			swap_endian(T value) noexcept
		{
			return internal::common_swap_endian(value);
		}

		template <typename T>
		LINGO_CONSTEXPR11 typename std::enable_if<
			std::is_integral<T>::value &&
			std::is_signed<T>::value &&
			sizeof(T) == 4, T>::type
			swap_endian(T value) noexcept
		{
			return internal::common_swap_endian(value);
		}

		template <typename T>
		LINGO_CONSTEXPR11 typename std::enable_if<
			std::is_integral<T>::value &&
			std::is_signed<T>::value &&
			sizeof(T) == 8, T>::type
			swap_endian(T value) noexcept
		{
			return internal::common_swap_endian(value);
		}
	}
}

#endif