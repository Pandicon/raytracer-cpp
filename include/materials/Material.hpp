#pragma once
#include <optional>

#include "Colour.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"

class Material
{
public:
    virtual ~Material() = default;

    virtual std::optional<Ray> scatter(const Ray &ray, const Vec3 &hit_point, const Vec3 &normal) const = 0;
    virtual Colour colour_contribution(const Ray &ray) const = 0;
};