#pragma once

namespace prosper
{

	template <typename T, size_t N>
	struct Vector
	{
		T fields[N]{};

		constexpr const T& operator[](const int& index) const
		{
			return this->fields[index];
		}

		T& operator[](const int& index)
		{
			return this->fields[index];
		}
	};
}