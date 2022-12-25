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

        // void translate(const Vector<T, N - 1> &v)
        //{
        //     for (int i = 0; i < N - 1; i++)
        //     {
        //         elements[N - 1][i] = v[i];
        //     }
        // }

    private:
        T elements[N][N];
    };
}