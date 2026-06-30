#pragma once

#include "Colour.hpp"
#include "Material.hpp"

class Mirror : public Material
{
private:
    Colour colour_;

public:
    Mirror(Colour colour) : colour_(colour) {}

    std::optional<Ray> scatter(const Ray &ray, const Vec3 &hit_point, const Vec3 &normal) const override
    {
        Vec3 reflected_direction = ray.direction.reflect(normal);
        Vec3 origin = hit_point;
        return Ray(origin, reflected_direction);
    }

    Colour colour_contribution(const Ray &_ray) const override
    {
        return colour_;
    };
};