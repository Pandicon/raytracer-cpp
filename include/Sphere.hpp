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
    uint32_t material_id_;

public:
    Sphere(Vec3 center, float radius, uint32_t material_id);
    std::optional<HitRecord> hit(const Ray &ray) const;
};