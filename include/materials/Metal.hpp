#pragma once

#include <optional>

#include "Colour.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"

class Metal
{
private:
    Colour colour_;
    double albedo_;
    double roughness_;

public:
    Metal(Colour colour, double albedo, double roughness);

    std::optional<Ray> scatter(const Ray &ray, const Vec3 &hit_point, const Vec3 &normal) const;

    Colour colour_contribution(const Ray &_ray) const;

    Colour emitted(const Ray &_ray) const;
};