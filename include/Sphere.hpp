#pragma once

#include <memory>

#include "Hittable.hpp"
#include "materials/Material.hpp"
#include "Vec3.hpp"

class Sphere : public Hittable
{
private:
    double radius_;
    Vec3 centre_;
    std::shared_ptr<Material> material_;

public:
    Sphere(Vec3 center, float radius, std::shared_ptr<Material> material);
    std::optional<HitRecord> hit(const Ray &ray) const override;
    std::optional<Ray> scatter(const Ray &ray) const override;
    Colour colour_contribution(const Ray &ray) const override;
};