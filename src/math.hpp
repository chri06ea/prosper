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

        static Matrix Identity()
        {
            return Matrix(1);
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

        T Length() const
        {
            T lengthSquared = 0;
            for (int i = 0; i < N; i++)
            {
                lengthSquared += elements[i] * elements[i];
            }
            return sqrt(lengthSquared);
        }

        void Normalize()
        {
            T length = Length();
            for (int i = 0; i < N; i++)
            {
                elements[i] /= length;
            }
        }

    private:
        T elements[N];
    };
}