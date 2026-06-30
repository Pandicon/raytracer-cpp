#pragma once

#include <algorithm>

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

    static Colour white()
    {
        return Colour(1.0, 1.0, 1.0, 1.0);
    }

    inline Colour operator+(Colour other) const
    {
        return Colour(r + other.r, g + other.g, b + other.b, a + other.a);
    }

    inline Colour operator*(Colour other) const
    {
        return Colour(r * other.r, g * other.g, b * other.b, a * other.a);
    }

    inline Colour operator*(double n) const
    {
        return Colour(r * n, g * n, b * n, a * n);
    }

    inline Colour operator/(double n) const
    {
        return Colour(r / n, g / n, b / n, a / n);
    }

    inline uint32_t pack() const
    {
        double a_ = std::min(std::max(a, 0.0), 1.0);
        double r_ = std::min(std::max(r, 0.0), 1.0);
        double g_ = std::min(std::max(g, 0.0), 1.0);
        double b_ = std::min(std::max(b, 0.0), 1.0);

        uint8_t final_a = static_cast<uint8_t>(a_ * 255.999);
        uint8_t final_r = static_cast<uint8_t>(r_ * 255.999);
        uint8_t final_g = static_cast<uint8_t>(g_ * 255.999);
        uint8_t final_b = static_cast<uint8_t>(b_ * 255.999);
        return final_a << 24 | final_r << 16 | final_g << 8 | final_b << 0;
    };
};