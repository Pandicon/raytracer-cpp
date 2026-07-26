#include "materials/Light.hpp"

Light::Light(ColourRGB colour, double intensity) : colour_(colour), intensity_(intensity) {}

std::optional<Ray> Light::scatter(const Ray &_ray, const Vec3 &_hit_point, const Vec3 &_normal) const
{
    // The ray hit a light source, it should not continue
    return std::nullopt;
}

ColourRGB Light::colour_contribution(const Ray &_ray) const
{
    return ColourRGB::white();
};

ColourRGB Light::emitted(const Ray &_ray) const
{
    return colour_ * intensity_;
};