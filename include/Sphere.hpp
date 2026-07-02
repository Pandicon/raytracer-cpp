#pragma once

#include <memory>

#include "Colour.hpp"
#include "HitRecord.hpp"
#include "materials/Material.hpp"
#include "Vec3.hpp"

class Sphere
{
private:
    double radius_;
    Vec3 centre_;
    std::shared_ptr<Material> material_;

public:
    Sphere(Vec3 center, float radius, std::shared_ptr<Material> material);
    std::optional<HitRecord> hit(const Ray &ray) const;
    std::optional<Ray> scatter(const Ray &ray, const Vec3 &hit_point, const Vec3 &normal) const;
    Colour colour_contribution(const Ray &ray) const;
    Colour emitted(const Ray &ray) const;
};