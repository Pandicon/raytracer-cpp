#pragma once

#include "Hittable.hpp"
#include "Vec3.hpp"

class Sphere : public Hittable
{
private:
    double radius_;
    Vec3 centre_;

public:
    Sphere(Vec3 center, float radius);
    std::optional<HitRecord> hit(const Ray &ray) const override;
};