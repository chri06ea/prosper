#pragma once

namespace prosper
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

		// Matrix scaling overload
		template <typename T>
		constexpr const auto operator *(const T& rhs) const
		{
			Matrix<T, Rows, Columns> result{};

			for(auto i = 0; i < Rows * Columns; i++)
				result[i] = (*this)[i] * rhs;

			return result;
		}

		// Matrix multiplication
		template <size_t RhsRows, size_t RhsColumns>
		constexpr const auto operator*(const Matrix<T, RhsRows, RhsColumns>& rhs) const
		{
			// For now, only allow matrix multiplication of unidentical matrices.

			static_assert(Columns == RhsRows);

			Matrix<T, RhsColumns, Rows> result{};

			int i{};

			for(size_t a_row = 0; a_row < Rows; a_row++)
			{
				for(size_t b_column = 0; b_column < RhsColumns; b_column++)
				{
					float dot = 0.f;
					for(size_t i = 0; i < Columns; i++)
					{
						dot += (*this)(a_row, i) * rhs(i, b_column);
					}
					result[i++] = dot;
				}
			}
			return result;
		}
	};

	namespace tests
	{
		namespace t1
		{
			// test 1d access
			constexpr auto m13 = Matrix<float, 1, 3>{0,1,2};
			static_assert(m13.Width == 3 && m13.Height == 1);
			static_assert(m13[0] == 0.f && m13[1] == 1.f && m13[2] == 2.f);
			static_assert(m13(0, 0) == 0.f && m13(0, 1) == 1.f && m13(0, 2) == 2.f);
		}

		namespace t2
		{
			// test 2d access
			constexpr auto m33 = Matrix<float, 3, 3>{0, 1, 2, 3, 4, 5, 6, 7, 8};
			static_assert(m33.Width == 3 && m33.Height == 3);
			static_assert(m33(0, 0) == 0.f && m33(0, 1) == 1.f && m33(0, 2) == 2.f);
			static_assert(m33(1, 0) == 3.f && m33(1, 1) == 4.f && m33(1, 2) == 5.f);
		}

		namespace t3
		{
			// Square matrices multiplication

			constexpr auto m22_0 = Matrix<float, 2, 2>{
				1, 2,
				3, 4};

			constexpr auto m22_1 = Matrix<float, 2, 2>{
				5, 6,
				7, 8};

			constexpr auto expected_result = Matrix<float, 2, 2>{
				19.f,   22.f,
				43.f,   50.f};

			static_assert(m22_0* m22_1 == expected_result);
		}

		namespace t4
		{
			// Non-square matrices multiplication

			constexpr auto m32 = Matrix<float, 3, 2>{
				1,   2,
				3,   4,
				5,   6};

			constexpr auto m23 = Matrix<float, 2, 3>{
				7,  8,  9,
				10, 11, 12};

			constexpr auto expected_result = Matrix<float, 3, 3>{
				27.f,   30.f,	33.f,
				61.f,   68.f,	75.f,
				95.f,   106.f,	117.f};


			static_assert(m32* m23 == expected_result);
		}


		namespace t5
		{
			// Matrix scaling
			constexpr auto scaling = 5.f;

			constexpr auto m32 = Matrix<float, 3, 2>{
				1,   2,
				3,   4,
				5,   6};

			constexpr auto expected_result = Matrix<float, 3, 2>{
				1 * scaling,   2 * scaling,
				3 * scaling,   4 * scaling,
				5 * scaling,   6 * scaling};

			static_assert(m32 * 5.f == expected_result);
		}
	}
}