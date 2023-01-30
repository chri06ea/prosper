#include <platform/models/vector.hpp>

#include <platform/core/defs.hpp>

namespace lib
{
	using V3 = Vector3;

	constexpr auto operator+(const V3& lhs, const V3& rhs)
	{
		return V3{
			lhs.x + rhs.x,
			lhs.y + rhs.y,
			lhs.z + rhs.z,
		};
	}

	constexpr auto operator+=(V3& lhs, const V3& rhs)
	{
		lhs.x += rhs.x;
		lhs.y += rhs.y;
		lhs.z += rhs.z;
	}

	constexpr auto operator-(const V3& lhs, const V3& rhs)
	{
		return V3{
			lhs.x - rhs.x,
			lhs.y - rhs.y,
			lhs.z - rhs.z,
		};
	}
	constexpr auto operator*(const V3& lhs, const V3& rhs)
	{
		return V3{
			lhs.x * rhs.x,
			lhs.y * rhs.y,
			lhs.z * rhs.z,
		};
	}
	constexpr auto operator/(const V3& lhs, const V3& rhs)
	{
		return V3{
			lhs.x / rhs.x,
			lhs.y / rhs.y,
			lhs.z / rhs.z,
		};
	}

	constexpr auto operator*(const V3& lhs, const float& rhs)
	{
		return V3{
			lhs.x * rhs,
			lhs.y * rhs,
			lhs.z * rhs,
		};
	}

	constexpr auto operator/(const V3& lhs, const float& rhs)
	{
		return V3{
			lhs.x / rhs,
			lhs.y / rhs,
			lhs.z / rhs,
		};
	}

	constexpr const float length_sqr(const V3& vec)
	{
		return {
			vec.x * vec.x +
			vec.y * vec.y +
			vec.z * vec.z
		};
	}

	constexpr const float length(const V3& vec)
	{
		return csqrt(length_sqr(vec));
	}

	constexpr const V3 normalized(const V3& vec)
	{
		const auto len = length(vec);
		if(len == 0.f) return {};
		return vec / len;
	}
}