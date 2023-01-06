#pragma once

#include "mathfn.hpp"

namespace prosper
{
	template <typename T, size_t N>
	struct VectorBase
	{
		T fields[N]{};

		// Index accesor
		constexpr const T& operator[](const int& index) const
		{
			return fields[index];
		}

		// Index accesor
		constexpr T& operator[](const int& index)
		{
			return fields[index];
		}

		// Compare vectors
		constexpr const auto operator ==(const VectorBase<T, N>& rhs) const
		{
			for(auto i = 0; i < N; i++)
				if(fields[i] != rhs[i])
					return false;

			return true;
		}

		// Add vectors
		constexpr const auto operator +(const VectorBase<T, N>& rhs) const
		{
			VectorBase<T, N> result{};
			for(auto i = 0; i < N; i++)
				result[i] = fields[i] + rhs[i];
			return result;
		}

		// Subtract vectors
		constexpr const auto operator -(const VectorBase<T, N>& rhs) const
		{
			VectorBase<T, N> result{};
			for(auto i = 0; i < N; i++)
				result[i] = fields[i] - rhs[i];
			return result;
		}


		// Multiply vectors
		constexpr const auto operator *(const VectorBase<T, N>& rhs) const
		{
			VectorBase<T, N> result{};
			for(size_t i = 0; i < N; i++)
				result[i] = fields[i] * rhs[i];
			return result;
		}

		// Divide vectors
		constexpr const auto operator /(const VectorBase<T, N>& rhs) const
		{
			VectorBase<T, N> result{};
			for(auto i = 0; i < N; i++)
				result[i] = fields[i] / rhs[i];
			return result;
		}

		// Up scale
		//template <typename U>
		constexpr const auto operator *(const float& rhs) const
		{
			VectorBase<T, N> result{};
			for(auto i = 0; i < N; i++)
				result[i] = fields[i] * rhs;
			return result;
		}

		// Down scale
		//template <typename U>
		constexpr const auto operator /(const float& rhs) const
		{
			VectorBase<T, N> result{};
			for(auto i = 0; i < N; i++)
				result[i] = fields[i] / rhs;
			return result;
		}


		// Squared length
		constexpr const T length_sqr() const
		{
			T length_squared{};
			for(int i = 0; i < N; i++)
				length_squared += fields[i] * fields[i];
			return length_squared;
		}

		// Length
		constexpr const T length() const
		{
			return csqrt(length_sqr());
		}

		// Normalize
		constexpr const auto normalized() const
		{
			VectorBase<T, N> result = *this;
			const auto len = length();
			for(size_t i = 0; i < N; i++)
				result[i] /= len;
			return result;
		}
	};

	template <typename T, size_t N>
	struct Vector : VectorBase<T, N> {};

	template <typename T>
	struct Vector<T, 2> : VectorBase<T, 2>
	{
		constexpr Vector()
		{
			this->fields[0] = T{}, this->fields[1] = T{};
		}

		constexpr Vector(T x, T y)
		{
			this->fields[0] = x, this->fields[1] = y;
		}

		constexpr T& x() { return this->fields[0]; }
		constexpr T& y() { return this->fields[1]; }
		constexpr const T& x() const { return this->fields[0]; }
		constexpr const T& y() const { return this->fields[1]; }
	};


	template <typename T>
	struct Vector<T, 3> : VectorBase<T, 3>
	{
		constexpr Vector()
		{
			this->fields[0] = T{}, this->fields[1] = T{}, this->fields[2] = T{};
		}

		constexpr Vector(T x, T y, T z)
		{
			this->fields[0] = x, this->fields[1] = y, this->fields[2] = z;
		}

		constexpr T& x() { return this->fields[0]; }
		constexpr T& y() { return this->fields[1]; }
		constexpr T& z() { return this->fields[2]; }
		constexpr const T& x() const { return this->fields[0]; }
		constexpr const T& y() const { return this->fields[1]; }
		constexpr const T& z() const { return this->fields[2]; }
	};

	template <typename T>
	using Vector2 = Vector<T, 2>;


	using Vector2i = Vector<int, 2>;
	using Vector2f = Vector<float, 2>;
	using Vector2d = Vector<double, 2>;


	template <typename T>
	using Vector3 = Vector<T, 3>;

	using Vector3i = Vector<int, 3>;
	using Vector3f = Vector<float, 3>;
	using Vector3d = Vector<double, 3>;

	namespace tests
	{
		namespace vector
		{
			constexpr auto x = Vector<float, 3>{2, 3.6, 2.1}.x();
			constexpr auto y = Vector<float, 3>{2, 3.6, 2.1}.y();
			constexpr auto z = Vector<float, 3>{2, 3.6, 2.1}.z();

			using V = Vector<float, 3>;

			static_assert(V{2, 2, 2} *2.f == V{4,4,4});
			static_assert(V{4, 4, 4} / 2.f == V{2,2,2});

			static_assert(V{2,4,6} + V{4,3,1} == V{6,7,7});
			static_assert(V{3,3,3} - V{1,1,1} == V{2,2,2});
			static_assert(V{2,4,6} *V{4,3,1} == V{8,12,6});
			static_assert(V{2,9,6} / V{4,3,1} == V{0.5,3,6});

			static_assert(V{2,2,2}.length() == 3.4641015f);
			static_assert(V{5,3,8}.length() == 9.899495f);

			static_assert(V{1,2,3}.normalized() == V{0.2672612369f, 0.534522478f, 0.8017836809f});
		}
	}
}