#pragma once

#include <optional>

#include "external/rgb2spec/rgb2spec.h"

#include "Colour.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"

class Diffuse
{
private:
    float coefficients_[3];

public:
    Diffuse(ColourRGB colour, RGB2Spec *rgb2spec);

    std::optional<Ray> scatter(const Ray &ray, const Vec3 &hit_point, const Vec3 &normal) const;

    double albedo(const Ray &_ray, double lambda) const;

    double emitted(const Ray &_ray) const;
};