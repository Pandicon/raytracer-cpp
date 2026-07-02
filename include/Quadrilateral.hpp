#pragma once

#include "Colour.hpp"
#include "HitRecord.hpp"
#include "materials/Material.hpp"
#include "Vec3.hpp"

class Quadrilateral
{
private:
    Vec3 Q_;
    Vec3 u_;
    Vec3 v_;
    Vec3 u_cross_v_;
    double u_cross_v_len_squared;
    Vec3 u_norm_;
    Vec3 v_norm_;
    Vec3 normal_;
    double plane_d_;

    Material material_;

public:
    Quadrilateral(Vec3 Q, Vec3 u, Vec3 v, Material material);
    std::optional<HitRecord> hit(const Ray &ray) const;
};