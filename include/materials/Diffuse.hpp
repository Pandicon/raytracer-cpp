#pragma once

#include "Colour.hpp"
#include "Material.hpp"

class Diffuse : public Material
{
private:
    Colour colour_;

public:
    Diffuse(Colour colour);

    std::optional<Ray> scatter(const Ray &ray, const Vec3 &hit_point, const Vec3 &normal) const override;

    Colour colour_contribution(const Ray &_ray) const override;

    Colour emitted(const Ray &_ray) const override;
};