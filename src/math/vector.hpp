namespace prosper
{
    template <typename T, int N>
    class Vector
    {
    public:
        constexpr Vector()
        {
            for (auto &e : elements)
                e = T{};
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

    protected:
        T elements[N]{};
    };

    class FVec2 : public Vector<float, 2>
    {
    public:
        float &x() { return elements[0]; }
        // const float &x() { return elements[0]; }
        float &y() { return elements[1]; }
        // const float &y() { return elements[1]; }
    };

    class FVec3 : public Vector<float, 3>
    {
    public:
        constexpr FVec3()
        {
            elements[0] = 0.f;
            elements[1] = 0.f;
            elements[2] = 0.f;
        }
        constexpr FVec3(float x, float y, float z)
        {
            elements[0] = x;
            elements[1] = y;
            elements[2] = z;
        }

        float &x() { return elements[0]; }
        // const float &x() { return elements[0]; }
        float &y() { return elements[1]; }
        // const float &y() { return elements[1]; }
        float &z() { return elements[0]; }
        // const float &z() { return elements[0]; }
    };
}