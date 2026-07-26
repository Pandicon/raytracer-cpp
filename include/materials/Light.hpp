#pragma once

#include <optional>

#include "Colour.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"

class Light
{
private:
    ColourRGB colour_;
    double intensity_;

public:
    Light(ColourRGB colour, double intensity);

    std::optional<Ray> scatter(const Ray &_ray, const Vec3 &_hit_point, const Vec3 &_normal) const;

    ColourRGB colour_contribution(const Ray &_ray) const;

    ColourRGB emitted(const Ray &_ray) const;
};