#include <cstdint>

struct Colour
{
    double r;
    double g;
    double b;
    double a;

    Colour(double r_, double g_, double b_, double a_) : r(r_), g(g_), b(b_), a(a_) {}

    inline Colour operator+(Colour other)
    {
        return Colour(r + other.r, g + other.g, b + other.b, a + other.a);
    }

    inline Colour operator*(double n)
    {
        return Colour(r * n, g * n, b * n, a * n);
    }

    inline Colour operator/(double n)
    {
        return Colour(r / n, g / n, b / n, a / n);
    }

    inline uint32_t pack()
    {
        return ((uint8_t)a) << 24 | ((uint8_t)r) << 16 | ((uint8_t)g) << 8 | ((uint8_t)b) << 0;
    };
};