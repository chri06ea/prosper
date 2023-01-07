#pragma once

#include <vector>

namespace prosper
{
	template <typename T>
	struct List
	{
		constexpr List()
		{

		}

		template <size_t N>
		constexpr List(T(&items)[N])
		{

		}
	};

	template <typename T>
	struct DynamicList : std::vector<T>
	{
	};

	template <typename T, size_t N>
	struct StaticList : List<T>
	{
		std::array<T, N> elements{};
		size_t slots_used{}, capacity = N;

		// Constructor

		constexpr StaticList()
		{

		}

		template <typename T>
		constexpr StaticList()
		{

		}


		// Element accessor

		constexpr T& operator[](const int& index)
		{
			return elements[index];
		}

		constexpr const T& operator[](const int& index) const
		{
			return elements[index];
		}

		// Initialize from a list of elements
		template <typename ...Ts>
		constexpr StaticList(const Ts&...ts)
			: elements({ts...}), slots_used(sizeof...(ts))
		{
		}

		// Equality
		template <typename RhsType, size_t RhsCount>
		constexpr const auto operator==(const StaticList<RhsType, RhsCount>& other) const
		{
			if constexpr(std::is_same_v<RhsType, T> && N == RhsCount)
			{
				if(slots_used != other.slots_used)
					return false;

				for(auto i = 0; i < N; i++)
					if(elements[i] != other[i])
						return false;

				return true;
			}

			return false;
		}

		// Add something at the end of the list
		constexpr T& push(const T& value)
		{
			assert(slots_used < capacity);
			elements[slots_used] = value;
			return elements[slots_used++];
		}

		// Remove an item at the end of the list
		constexpr void pop()
		{
			slots_used--;
		}
	};

}
