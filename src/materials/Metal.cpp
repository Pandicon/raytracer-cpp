#include "materials/Metal.hpp"

Metal::Metal(Colour colour, double albedo, double roughness) : colour_(colour), albedo_(albedo), roughness_(roughness) {}

std::optional<Ray> Metal::scatter(const Ray &ray, const Vec3 &hit_point, const Vec3 &normal) const
{
    Vec3 reflected_direction = ray.direction.reflect(normal);
    Vec3 new_direction = reflected_direction + Vec3::random_unit_vector() * roughness_;
    if (new_direction.dot(normal) <= 0.0)
    {
        // TODO: This approximation is not really correct...
        return std::nullopt;
    }
    Vec3 origin = hit_point;
    return Ray(origin, new_direction.normalise());
}

Colour Metal::colour_contribution(const Ray &_ray) const
{
    return colour_ * albedo_;
};

Colour Metal::emitted(const Ray &_ray) const
{
    return Colour::black();
};