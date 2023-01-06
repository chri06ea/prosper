#pragma once
#include <limits>

//  https://stackoverflow.com/a/34134071

static inline constexpr double sqrt_newton_raphson(double x, double curr, double prev)
{
	return curr == prev
		? curr
		: sqrt_newton_raphson(x, 0.5 * (curr + x / curr), curr);
}

static inline constexpr double csqrt(double x)
{
	return x >= 0 && x < std::numeric_limits<double>::infinity()
		? sqrt_newton_raphson(x, x, 0)
		: std::numeric_limits<double>::quiet_NaN();
}

namespace tests
{
	namespace math_functions
	{
		static_assert(csqrt(9) == 3);
		static_assert(csqrt(16) == 4);
	}
}