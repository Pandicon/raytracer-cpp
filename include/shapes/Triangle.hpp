#pragma once

#include "Colour.hpp"
#include "HitRecord.hpp"
#include "Material.hpp"
#include "Vec3.hpp"

class Triangle
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

    uint32_t material_id_;

public:
    Triangle(Vec3 Q, Vec3 u, Vec3 v, uint32_t material_id);
    // Three vertices in CCW order
    static Triangle from_vertices(Vec3 A, Vec3 B, Vec3 C, uint32_t material_id);
    std::optional<HitRecord> hit(const Ray &ray) const;
};