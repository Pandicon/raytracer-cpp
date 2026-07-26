#pragma once

#include <algorithm>

#include "colour_tables.hpp"

struct ColourRGB
{
    double r;
    double g;
    double b;

    ColourRGB(double r_, double g_, double b_) : r(r_), g(g_), b(b_) {}

    static ColourRGB fromRGB(int r_, int g_, int b_)
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

struct ColourXYZ
{
    double x;
    double y;
    double z;

    constexpr ColourXYZ(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}

    static constexpr ColourXYZ from_lambda_intensity(double lambda, double intensity, double bin_width)
    {
        // These values are invisible
        if (lambda < CIE_1931_CMF_MIN_LAMBDA || lambda >= (CIE_1931_CMF_MIN_LAMBDA + CIE_1931_CMF_STEPS * CIE_1931_CMF_LAMBDA_STEP))
        {
            return ColourXYZ(0.0, 0.0, 0.0);
        }
        const double exact_idx = (lambda - CIE_1931_CMF_MIN_LAMBDA) / CIE_1931_CMF_LAMBDA_STEP;
        const uint32_t i = static_cast<uint32_t>(exact_idx);
        if (i == CIE_1931_CMF_STEPS - 1)
        {
            return ColourXYZ(CIE_1931_CMF[CIE_1931_CMF_STEPS - 1][0], CIE_1931_CMF[CIE_1931_CMF_STEPS - 1][1], CIE_1931_CMF[CIE_1931_CMF_STEPS - 1][2]) * intensity * bin_width;
        }
        const double t = exact_idx - i;
        const ColourXYZ c0 = ColourXYZ(CIE_1931_CMF[i][0], CIE_1931_CMF[i][1], CIE_1931_CMF[i][2]);
        const ColourXYZ c1 = ColourXYZ(CIE_1931_CMF[i + 1][0], CIE_1931_CMF[i + 1][1], CIE_1931_CMF[i + 1][2]);
        return (c0 * (1 - t) + c1 * t) * intensity * bin_width;
    }

    inline ColourXYZ operator+(ColourXYZ other) const
    {
        return ColourXYZ(x + other.x, y + other.y, z + other.z);
    }

    inline ColourXYZ operator*(ColourXYZ other) const
    {
        return ColourXYZ(x * other.x, y * other.y, z * other.z);
    }

    inline ColourXYZ operator*(double n) const
    {
        return ColourXYZ(x * n, y * n, z * n);
    }

    inline ColourXYZ operator/(double n) const
    {
        return ColourXYZ(x / n, y / n, z / n);
    }

    constexpr inline double dot(ColourXYZ other) const
    {
        return x * other.x + y * other.y + z * other.z;
    }

    inline ColourRGB to_rgb() const
    {
        static constexpr ColourXYZ r_v = ColourXYZ(3.2404542, -1.5371385, -0.4985314);
        static constexpr ColourXYZ g_v = ColourXYZ(-0.9692660, 1.8760108, 0.0415560);
        static constexpr ColourXYZ b_v = ColourXYZ(0.0556434, -0.2040259, 1.0572252);

        return ColourRGB(this->dot(r_v), this->dot(g_v), this->dot(b_v));
    }
};