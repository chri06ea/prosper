namespace prosper
{
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
    class FVec2 : public Vector<float, 2>
    {
    public:
        float &x() { return elements[0]; }
        const float &x() { return elements[0]; }
        float &y() { return elements[1]; }
        const float &y() { return elements[1]; }
    };

    template <typename T>
    class FVec3 : public Vector<float, 3>
    {
    public:
        float &x() { return elements[0]; }
        const float &x() { return elements[0]; }
        float &y() { return elements[1]; }
        const float &y() { return elements[1]; }
        float &z() { return elements[0]; }
        const float &z() { return elements[0]; }
    };
}