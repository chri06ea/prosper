#include <platform/models/matrix.hpp>
#include <platform/models/point.hpp>

namespace lib
{
	template <typename T, size_t Rows, size_t Columns>
	constexpr const auto operator *(const Matrix<T, Rows, Columns>& lhs, const T& rhs)
	{
		Matrix<T, Rows, Columns> result{};

		for(auto i = 0; i < Rows * Columns; i++)
			result[i] = lhs[i] * rhs;

		return result;
	}

	template <typename T, size_t Rows, size_t Columns, size_t RhsRows, size_t RhsColumns>
	constexpr const auto operator*(
		const Matrix<T, Rows, Columns>& lhs,
		const Matrix<T, RhsRows, RhsColumns>& rhs)
	{
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
					dot += lhs(a_row, i) * rhs(i, b_column);
				}
				result[i++] = dot;
			}
		}
		return result;
	}

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
		return Point<float>{
			normalized_device_coordinates(0, 0),
			normalized_device_coordinates(1, 0),
		};
	}
}