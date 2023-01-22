#pragma once

namespace lib
{
	template <typename T, int Rows, int Columns>
	struct Matrix
	{
		static constexpr auto Width = Columns;
		static constexpr auto Height = Rows;
		static constexpr auto NumElements = Rows * Columns;

		T elements[Rows][Columns]{};

		// Field accessor
		constexpr T& operator()(size_t row, size_t column)
		{
			return elements[row][column];
		}

		// Const field accessor
		constexpr const T operator()(size_t row, size_t column) const
		{
			return elements[row][column];
		}

		// Field accessor
		constexpr T& operator[](const int index)
		{
			return elements[index / Columns][index % Columns];
		}

		// Const field accessor
		constexpr const T operator[](const int& index) const
		{
			return elements[index / Columns][index % Columns];
		}

		// Equality check
		constexpr const auto operator==(const Matrix<T, Rows, Columns>& rhs) const
		{
			for(auto i = 0; i < Rows * Columns; i++)
				if((*this)[i] != rhs[i])
					return false;

			return true;
		}
	};
}