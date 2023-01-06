#pragma once

#include "vector.hpp"
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

	static inline constexpr auto project_orthographic(float screen_x, float screen_y, float screen_width, float screen_height)
	{
		constexpr auto model = TranslationMatrix(0, 0, 0);
		auto projection_matrix = OrthographicMatrix(0.f, screen_width, 0.f, screen_height, -1.f, 1.f);
		auto normalized_device_coordinates = Matrix<float, 1, 4>{screen_x, screen_y, 0, 1} *projection_matrix;
		return Vector2<float>{
			normalized_device_coordinates(0, 0),
			normalized_device_coordinates(1, 0),
		};
	}

	namespace tests
	{
		namespace matrix
		{
			namespace projection
			{
				constexpr auto value = project_orthographic(400, 200, 800, 400);
				constexpr auto expected_result = Vector2f{0.f, 0.f};
				static_assert(value == expected_result);
			}
		}
	}
}