#pragma once

#include "Colour.hpp"
#include "Hittable.hpp"
#include "materials/Material.hpp"
#include "Vec3.hpp"

class Quadrilateral : public Hittable
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

    std::shared_ptr<Material> material_;

public:
    Quadrilateral(Vec3 Q, Vec3 u, Vec3 v, std::shared_ptr<Material> material);
    std::optional<HitRecord> hit(const Ray &ray) const override;
    std::optional<Ray> scatter(const Ray &ray, const Vec3 &hit_point, const Vec3 &normal) const override;
    Colour colour_contribution(const Ray &ray) const override;
    Colour emitted(const Ray &ray) const override;
};