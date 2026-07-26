#pragma once

#include <optional>

#include "Colour.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"

class Diffuse
{
private:
    ColourRGB colour_;

public:
    Diffuse(ColourRGB colour);

    std::optional<Ray> scatter(const Ray &ray, const Vec3 &hit_point, const Vec3 &normal) const;

    ColourRGB colour_contribution(const Ray &_ray) const;

    ColourRGB emitted(const Ray &_ray) const;
};