#pragma once

#include "matrix.hpp"

namespace prosper
{
	constexpr inline Matrix<float, 4, 4> OrthographicMatrix(
		float left, float right,
		float bottom, float top, 
		float near, float far)
	{
		return Matrix<float, 4, 4>{
			2.0f / (right - left), 0.0f, 0.0f, 0.0f,
			0.0f, 2.0f / (top - bottom), 0.0f, 0.0f,
			0.0f, 0.0f, -2.0f / (far - near), 0.0f,
			-(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1.0f,
		};
	};

	constexpr inline Matrix<float, 4, 4> TranslationMatrix(
		float x, float y, float z)
	{
		return Matrix<float, 4, 4>{
			1.0, 0.0, 0.0, x,
			0.0, 1.0, 0.0, y,
			0.0, 0.0, 1.0, z,
			0.0, 0.0, 0.0, 1.0};
	}

	namespace tests
	{
		namespace matrix
		{
			namespace projection
			{
				constexpr auto projection_matrix = OrthographicMatrix(0, 800.f, 0, 400.f, -1.f, 1.f);
				constexpr auto model = TranslationMatrix(0, 0, 0);
				constexpr auto normalized_device_coordinates = Matrix<float, 1, 4>{200, 200, 0, 1} * projection_matrix;
				constexpr auto expected_result = Matrix<float, 4, 1>{-0.5f, 0.0f, 0.f, 1.f};
				static_assert(normalized_device_coordinates == expected_result);
			}
		}
	}
}