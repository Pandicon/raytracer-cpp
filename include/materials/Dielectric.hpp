#pragma once

#include <array>
#include <optional>
#include <vector>

#include "external/rgb2spec/rgb2spec.h"

#include "Colour.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"

class Dielectric
{
private:
    float coefficients_[3];

    static constexpr size_t MAX_TERMS = 4;
    std::array<double, MAX_TERMS> As_{};
    std::array<double, MAX_TERMS> Bs_{};
    uint8_t num_terms_ = 0;

public:
    Dielectric(ColourRGB colour, const std::vector<double> &As, const std::vector<double> &Bs, RGB2Spec *rgb2spec);

    std::optional<Ray> scatter(const Ray &ray, const Vec3 &hit_point, const Vec3 &normal, double n_previous_over_n_next, bool &refracted) const;

    double albedo(const Ray &_ray, double lambda) const;

    double emitted(const Ray &_ray) const;

    inline double index_of_refraction(double lambda) const
    {
        const double lambda_micro_m = lambda / 1000.0;
        const double l2 = lambda_micro_m * lambda_micro_m;
        double sum = 1.0;
        for (int i = 0; i < num_terms_; i += 1)
        {
            sum += As_[i] * l2 / (l2 - Bs_[i]);
        }
        return std::sqrt(sum);
    }
};