#pragma once

#include <optional>

#include "external/rgb2spec/rgb2spec.h"

#include "Colour.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"

class Dielectric
{
private:
    float coefficients_[3];
    double index_of_refraction_;

public:
    Dielectric(ColourRGB colour, double index_of_refraction, RGB2Spec *rgb2spec);

    std::optional<Ray> scatter(const Ray &ray, const Vec3 &hit_point, const Vec3 &normal, double n_previous_over_n_next, bool &refracted) const;

    double albedo(const Ray &_ray, double lambda) const;

    double emitted(const Ray &_ray) const;

    inline double index_of_refraction() const
    {
        return index_of_refraction_;
    }
};