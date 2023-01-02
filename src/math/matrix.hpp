#pragma once

#include <cstdint>
#include <cmath>

namespace prosper
{
	template <typename T, size_t N>
	struct Matrix
	{
		constexpr Matrix2()
		{
			for(size_t x = 0; x < N; x++)
				for(size_t y = 0; y < N; y++)
					elements[x][y] = {};
		}

		constexpr Matrix2(const T(&values)[N])
		{
			for(size_t x = 0; x < N; x++)
				for(size_t y = 0; y < N; y++)
					elements[x][y] = values[x + y];
		}

		static constexpr Matrix<T, N> diagonal(const T& value)
		{
			Matrix<T, N> mat;

			for(int i = 0; i < N; i++)
				mat[i][i] = value;

			return mat;
		}

		T elements[N][N]{};
	};

	template <typename T, size_t N>
	constexpr inline static T matrix_sum(const Matrix<T, N>& mat)
	{
		T sum{};

		for(const auto& element : mat.elements)
			sum += element;

		return sum;
	}

	static inline constexpr auto a = []()
	{
	};

	static inline constexpr auto _Matrix_Test01 = Matrix<float, 5>{};
	static inline constexpr auto _Matrix_Test02 = matrix_sum(Matrix<float, 2>({1.f, 2.f, 3.f, 4.f}));
	static inline constexpr auto _Matrix_Test03 = Matrix<float, 8>({2.f});
}