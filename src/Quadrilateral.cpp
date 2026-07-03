#include "Quadrilateral.hpp"

Quadrilateral::Quadrilateral(Vec3 Q, Vec3 u, Vec3 v, uint32_t material_id) : Q_(Q), u_(u), v_(v), material_id_(material_id)
{
    u_norm_ = u.normalise();
    v_norm_ = v.normalise();
    u_cross_v_ = u.cross(v);
    u_cross_v_len_squared = u_cross_v_.length_squared();
    normal_ = u.cross(v).normalise();
    plane_d_ = normal_.dot(Q);
};

std::optional<HitRecord> Quadrilateral::hit(const Ray &ray) const
{
    // Intersect ray with the plane containing the quadrilateral
    double denominator = ray.direction.dot(normal_);
    if (std::fabs(denominator) < 1e-8)
    {
        // Basically parallel to the plane
        return std::nullopt;
    }
    double lambda = (plane_d_ - ray.origin.dot(normal_)) / denominator;
    if (lambda <= 0.0)
    {
        return std::nullopt;
    }

    Vec3 hit_point = ray.point_at(lambda);

    Vec3 local_point = hit_point - Q_;
    double alpha = (local_point.cross(v_)).dot(u_cross_v_) / u_cross_v_len_squared;
    double beta = (u_.cross(local_point)).dot(u_cross_v_) / u_cross_v_len_squared;

    if (alpha < 0.0 || alpha > 1.0 || beta < 0.0 || beta > 1.0)
    {
        return std::nullopt;
    }
    if (denominator > 0.0)
    {
        return HitRecord{hit_point, normal_ * (-1.0), lambda, false, material_id_};
    }
    return HitRecord{hit_point, normal_, lambda, true, material_id_};
};