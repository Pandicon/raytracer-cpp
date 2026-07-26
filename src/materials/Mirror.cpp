#include "materials/Mirror.hpp"

Mirror::Mirror(ColourRGB colour) : colour_(colour) {}

std::optional<Ray> Mirror::scatter(const Ray &ray, const Vec3 &hit_point, const Vec3 &normal) const
{
    Vec3 reflected_direction = ray.direction.reflect(normal);
    Vec3 origin = hit_point;
    return Ray(origin, reflected_direction);
}

ColourRGB Mirror::colour_contribution(const Ray &_ray) const
{
    return colour_;
};

ColourRGB Mirror::emitted(const Ray &_ray) const
{
    return ColourRGB::black();
};