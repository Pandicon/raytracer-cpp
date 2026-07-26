#include "materials/Diffuse.hpp"

Diffuse::Diffuse(ColourRGB colour) : colour_(colour) {}

std::optional<Ray> Diffuse::scatter(const Ray &ray, const Vec3 &hit_point, const Vec3 &normal) const
{
    Vec3 scattered_direction = normal + Vec3::random_unit_vector();
    if (scattered_direction.is_close_to_zero())
    {
        scattered_direction = normal;
    }
    Vec3 origin = hit_point;
    return Ray(origin, scattered_direction.normalise());
}

ColourRGB Diffuse::colour_contribution(const Ray &_ray) const
{
    return colour_;
};

ColourRGB Diffuse::emitted(const Ray &_ray) const
{
    return ColourRGB::black();
};