#pragma once

#include <cstdint>
#include <iomanip>
#include <array>

#include "vector.hpp"
#include "matrix.hpp"
#include "projection.hpp"

#undef fear
#undef near

namespace prosper
{
	
	//struct Angle : Matrix<float, 1, 3>
	//{
	//	float& pitch() { return this->fields[0][0]; }
	//	float& yaw() { return this->fields[1][0]; }
	//	float& roll() { return this->fields[2][0]; }
	//	const float& pitch() const { return this->fields[0][0]; }
	//	const float& yaw() const { return this->fields[1][0]; }
	//	const float& roll() const { return this->fields[2][0]; }
	//};

	template <typename T>
	struct Rect
	{
		T x, y, w, h;
	};

	template <typename T>
	struct Point
	{
		T x, y;
	};

	struct Color
	{
		Color()
			: Color(1.f, 1.f, 1.f, 1.f)
		{

		}

		Color(float r, float g, float b, float a)
			: r(r), g(g), b(b), a(a)
		{

		}

		float r, g, b, a;
	};

	static inline const Color White = Color{1.f,1.f,1.f,1.f};
}
