#ifndef H_LINGO_COMPRESSED_PAIR
#define H_LINGO_COMPRESSED_PAIR

#include <type_traits>

namespace lingo
{
	namespace internal
	{
		template <typename T, bool Empty = std::is_empty<T>::value>
		struct compressed_pair_first;

		template <typename T>
		struct compressed_pair_first<T, false>
		{
			T _first;
		};

		template <typename T>
		struct compressed_pair_first<T, true>
		{
			static T _first;
		};

		template <typename T>
		T compressed_pair_first<T, true>::_first;

		template <typename T, bool Empty = std::is_empty<T>::value>
		struct compressed_pair_second;

		template <typename T>
		struct compressed_pair_second<T, false>
		{
			T _second;
		};

		template <typename T>
		struct compressed_pair_second<T, true>
		{
			static T _second;
		};

		template <typename T>
		T compressed_pair_second<T, true>::_second;
	}

	template <typename FirstT, typename SecondT>
	class compressed_pair : private internal::compressed_pair_first<FirstT>, private internal::compressed_pair_second<SecondT>
	{
		public:
		using first_type = FirstT;
		using second_type = SecondT;

		compressed_pair() = default;

		compressed_pair(first_type first, second_type second)
		{
			this->_first = first;
			this->_second = second;
		}

		first_type& first() noexcept
		{
			return const_cast<first_type&>(static_cast<const compressed_pair*>(this)->first());
		}

		const first_type& first() const noexcept
		{
			return this->_first;
		}

		second_type& second() noexcept
		{
			return const_cast<second_type&>(static_cast<const compressed_pair*>(this)->second());
		}

		const second_type& second() const noexcept
		{
			return this->_second;
		}
	};
}

#endif