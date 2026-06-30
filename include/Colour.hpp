#include <cstdint>

struct Colour
{
    double r;
    double g;
    double b;
    double a;

    Colour(double r_, double g_, double b_, double a_) : r(r_), g(g_), b(b_), a(a_) {}

    static Colour fromRGBA(int r_, int g_, int b_, int a_)
    {
        return Colour(((double)r_) / 255.0, ((double)g_) / 255.0, ((double)b_) / 255.0, ((double)a_) / 255.0);
    }

    inline Colour operator+(Colour other)
    {
        return Colour(r + other.r, g + other.g, b + other.b, a + other.a);
    }

    inline Colour operator*(Colour other)
    {
        return Colour(r * other.r, g * other.g, b * other.b, a * other.a);
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
        return ((uint8_t)(a * 255.0)) << 24 | ((uint8_t)(r * 255.0)) << 16 | ((uint8_t)(g * 255.0)) << 8 | ((uint8_t)(b * 255.0)) << 0;
    };
};