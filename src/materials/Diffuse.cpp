#include "materials/Diffuse.hpp"

Diffuse::Diffuse(Colour colour) : colour_(colour) {}

std::optional<Ray> Diffuse::scatter(const Ray &ray, const Vec3 &hit_point, const Vec3 &normal) const
{
    Vec3 scattered_direction = normal.random_vector_in_hemisphere_around();
    Vec3 origin = hit_point;
    return Ray(origin, scattered_direction);
}

Colour Diffuse::colour_contribution(const Ray &_ray) const
{
    return colour_;
};