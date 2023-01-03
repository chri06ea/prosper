#pragma once

#include <cstdint>
#include <iomanip>
#include <array>

namespace prosper
{
	template <typename T, size_t Rows, size_t Columns>
	struct Matrix
	{
		T fields[Columns][Rows]{};

		// Field accessor
		constexpr T& operator()(size_t row, size_t column)
		{
			return fields[column][row];
		}

		// Const field accessor
		constexpr const T operator()(size_t row, size_t column) const
		{
			return fields[column][row];
		}

		constexpr const auto operator+(const Matrix<T, Rows, Columns>& rhs) const
		{
			Matrix<T, Rows, Columns> result;

			// Multiply fieldwise

			for(size_t y = 0; y < Rows; y++)
				for(size_t x = 0; x < Columns; x++)
					result(x, y) = (*this)(x, y) + rhs(x, y);

			return result;
		}

		template < size_t RhsRows, size_t RhsColumns>
		constexpr const auto operator*(const Matrix<T, RhsRows, RhsColumns>& rhs) const
		{
			Matrix<T, Columns, RhsRows> result;

			// Multiply rows by columns

			for(size_t x = 0; x < Columns; x++) // For each of A's rows
			{
				for(size_t y = 0; y < RhsRows; y++) // Walk B's rows
				{
					float dot = 0.f;
					for(size_t i = 0; i < RhsColumns; i++) // Walk A and B's columns and rows
					{
						auto lhs_col_value = (*this)(i, x);
						auto rhs_row_value = rhs(y, i);

						dot += lhs_col_value * rhs_row_value;
					}
					result(x, y) = dot;
				}
			}
			return result;
		}

	};

	// Creates an orthographic matrix. this is for mapping screen coordinates to normalized device coordinates
	/*
	*
	*/

	constexpr inline Matrix<float, 4, 4> OrthographicMatrix(
		float left, float right,
		float bottom, float top,
		float near, float far)
	{
		return Matrix<float, 4, 4>{
			2.0f / (right - left), 0.0f, 0.0f, 0.0f, //
				0.0f, 2.0f / (top - bottom), 0.0f, 0.0f, //
				0.0f, 0.0f, -2.0f / (far - near), 0.0f,  //
				-(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1.0f, //
		};
	};

	constexpr inline Matrix<float, 4, 4> TranslationMatrix(
		float x, float y, float z)
	{
		return Matrix<float, 4, 4>{
			1.0, 0.0, 0.0, 0.0,
				0.0, 1.0, 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				x, y, z, 1.0
		};
	}

	namespace test
	{
		// Try to project screen coords 400, 200 to normalized device coordinates

		static constexpr auto projection_matrix = OrthographicMatrix(0, 800, 0, 400, -1, 1);
		static constexpr auto model = TranslationMatrix(0, 0, 0);
		static constexpr auto wpos = (projection_matrix * model) * Matrix<float, 1, 4>{400, 200, 0, 1};
		static_assert(wpos(0, 0) == 0.f && wpos(1, 0) == 0.f);
	}
}