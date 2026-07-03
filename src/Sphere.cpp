#include "Sphere.hpp"

Sphere::Sphere(Vec3 centre, float radius, uint32_t material_id) : centre_(centre), radius_(radius), material_id_(material_id) {}

std::optional<HitRecord> Sphere::hit(const Ray &ray) const
{
    Vec3 O = ray.origin;
    Vec3 D = ray.direction;
    Vec3 C = centre_;
    Vec3 O_minus_C = O - C;

    double a = D.dot(D);
    double b = D.dot(O_minus_C);
    double c = O_minus_C.dot(O_minus_C) - radius_ * radius_;

    double dd = b * b - a * c;
    if (dd < 0.0)
    {
        return std::nullopt;
    }
    double d = std::sqrt(dd);
    double s1 = (-b + d) / a;
    double s2 = (-b - d) / a;
    if (s2 > 0.0)
    {
        Vec3 hit_point = ray.point_at(s2);
        Vec3 normal = (hit_point - centre_).normalise();
        return HitRecord{hit_point, normal, s2, true, material_id_};
    }
    else if (s1 > 0.0)
    {
        // The further hit is always coming out of the sphere
        Vec3 hit_point = ray.point_at(s1);
        Vec3 outwards_normal = (hit_point - centre_).normalise();
        return HitRecord{hit_point, outwards_normal * (-1.0), s1, false, material_id_};
    }
    else
    {
        return std::nullopt;
    }
}