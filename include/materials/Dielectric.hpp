#pragma once

#include <optional>

#include "Colour.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"

class Dielectric
{
private:
    ColourRGB colour_;
    double index_of_refraction_;

public:
    Dielectric(ColourRGB colour, double index_of_refraction);

    std::optional<Ray> scatter(const Ray &ray, const Vec3 &hit_point, const Vec3 &normal, double n_previous_over_n_next, bool &refracted) const;

    ColourRGB colour_contribution(const Ray &_ray) const;

    ColourRGB emitted(const Ray &_ray) const;

    inline double index_of_refraction() const
    {
        return index_of_refraction_;
    }
};