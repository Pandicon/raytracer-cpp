#pragma once
#include <optional>

#include "Colour.hpp"
#include "Ray.hpp"

class Material
{
public:
    virtual ~Material() = default;

    virtual std::optional<Ray> scatter(const Ray &ray) const = 0;
    virtual Colour colour_contribution(const Ray &ray) const = 0;
};