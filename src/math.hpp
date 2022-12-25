#pragma once

#include <cmath>

namespace prosper
{
    template <typename T, int N>
    class Matrix
    {
    public:
        Matrix()
        {
            memset(elements, 0, sizeof(elements));
        }

        Matrix(T diagonal)
        {
            memset(elements, 0, sizeof(elements));
            for (int i = 0; i < N; i++)
            {
                elements[i][i] = diagonal;
            }
        }

        Matrix(const T (&elements)[N][N])
        {
            memcpy(this->elements, elements, sizeof(this->elements));
        }

        T *operator[](int row)
        {
            return elements[row];
        }

        const T *operator[](int row) const
        {
            return elements[row];
        }

        static Matrix identity()
        {
            return Matrix(1);
        }

        void translate(const Vector<T, N - 1> &v)
        {
            for (int i = 0; i < N - 1; i++)
            {
                elements[N - 1][i] = v[i];
            }
        }

    private:
        T elements[N][N];
    };

    template <typename T, int N>
    class Vector
    {
    public:
        Vector()
        {
            memset(elements, 0, sizeof(elements));
        }

        Vector(const T *elements)
        {
            memcpy(this->elements, elements, sizeof(this->elements));
        }

        T &operator[](int index)
        {
            return elements[index];
        }

        const T &operator[](int index) const
        {
            return elements[index];
        }

        T length() const
        {
            T lengthSquared = 0;
            for (int i = 0; i < N; i++)
            {
                lengthSquared += elements[i] * elements[i];
            }
            return sqrt(lengthSquared);
        }

        void normalize()
        {
            T length = length();
            for (int i = 0; i < N; i++)
            {
                elements[i] /= length;
            }
        }

    private:
        T elements[N];
    };

    template <typename T>
    class Vec2 : public Vector<float, 2>
    {
    public:
    };
}