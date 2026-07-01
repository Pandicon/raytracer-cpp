#pragma once

#include "Colour.hpp"
#include "Material.hpp"

class Light : public Material
{
private:
    Colour colour_;
    double intensity_;

public:
    Light(Colour colour, double intensity);

    std::optional<Ray> scatter(const Ray &_ray, const Vec3 &_hit_point, const Vec3 &_normal) const override;

    Colour colour_contribution(const Ray &_ray) const override;

    Colour emitted(const Ray &_ray) const override;
};