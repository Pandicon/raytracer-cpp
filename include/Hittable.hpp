#pragma once
#include <optional>

#include "Colour.hpp"
#include "HitRecord.hpp"
#include "Ray.hpp"

class Hittable
{
public:
    virtual ~Hittable() = default;

    virtual std::optional<HitRecord> hit(const Ray &ray) const = 0;
    virtual std::optional<Ray> scatter(const Ray &ray, const Vec3 &hit_point, const Vec3 &normal) const = 0;
    virtual Colour colour_contribution(const Ray &ray) const = 0;
    virtual Colour emitted(const Ray &ray) const = 0;
};