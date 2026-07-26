#include "materials/Mirror.hpp"

Mirror::Mirror(ColourRGB colour, RGB2Spec *rgb2spec)
{
    float rgb[3] = {static_cast<float>(colour.r), static_cast<float>(colour.g), static_cast<float>(colour.b)};
    rgb2spec_fetch(rgb2spec, rgb, coefficients_);
}

std::optional<Ray> Mirror::scatter(const Ray &ray, const Vec3 &hit_point, const Vec3 &normal) const
{
    Vec3 reflected_direction = ray.direction.reflect(normal);
    Vec3 origin = hit_point;
    return Ray(origin, reflected_direction);
}

double Mirror::albedo(const Ray &_ray, double lambda) const
{
    return rgb2spec_eval_precise(coefficients_, lambda);
};

double Mirror::emitted(const Ray &_ray) const
{
    return 0.0;
};