#pragma once

#include "Colour.hpp"
#include "Material.hpp"

class Light : public Material
{
private:
    Colour colour_;
    double intensity_;

public:
    Light(Colour colour, double intensity) : colour_(colour), intensity_(intensity) {}

    std::optional<Ray> scatter(const Ray &_ray) const override
    {
        // The ray hit a light source, it should not continue
        return std::nullopt;
    }

    Colour colour_contribution(const Ray &_ray) const override
    {
        return colour_ * intensity_;
    };
};