#pragma once

#include <limits>

// Constexpr sqrt
//  https://stackoverflow.com/a/34134071

static inline constexpr double sqrt_newton_raphson(float x, float curr, float prev)
{
	return curr == prev
		? curr
		: sqrt_newton_raphson(x, 0.5f * (curr + x / curr), curr);
}

static inline constexpr double csqrt(float x)
{
	return x >= 0.f && x < std::numeric_limits<float>::infinity()
		? sqrt_newton_raphson(x, x, 0.f)
		: std::numeric_limits<float>::quiet_NaN();
}