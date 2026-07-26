#pragma once

#include <algorithm>

struct ColourRGB
{
    double r;
    double g;
    double b;

    ColourRGB(double r_, double g_, double b_) : r(r_), g(g_), b(b_) {}

    static ColourRGB fromRGBA(int r_, int g_, int b_)
    {
        return ColourRGB(((double)r_) / 255.0, ((double)g_) / 255.0, ((double)b_) / 255.0);
    }

    static ColourRGB black()
    {
        return ColourRGB(0.0, 0.0, 0.0);
    }

    static ColourRGB white()
    {
        return ColourRGB(1.0, 1.0, 1.0);
    }

    inline ColourRGB operator+(ColourRGB other) const
    {
        return ColourRGB(r + other.r, g + other.g, b + other.b);
    }

    inline ColourRGB operator*(ColourRGB other) const
    {
        return ColourRGB(r * other.r, g * other.g, b * other.b);
    }

    inline ColourRGB operator*(double n) const
    {
        return ColourRGB(r * n, g * n, b * n);
    }

    inline ColourRGB operator/(double n) const
    {
        return ColourRGB(r / n, g / n, b / n);
    }

    inline uint32_t pack(uint8_t a) const
    {
        double r_ = std::min(std::max(r, 0.0), 1.0);
        double g_ = std::min(std::max(g, 0.0), 1.0);
        double b_ = std::min(std::max(b, 0.0), 1.0);

        uint8_t final_a = a;
        uint8_t final_r = static_cast<uint8_t>(r_ * 255.999);
        uint8_t final_g = static_cast<uint8_t>(g_ * 255.999);
        uint8_t final_b = static_cast<uint8_t>(b_ * 255.999);
        return final_a << 24 | final_r << 16 | final_g << 8 | final_b << 0;
    };
};