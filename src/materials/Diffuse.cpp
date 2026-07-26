#include "materials/Diffuse.hpp"

Diffuse::Diffuse(ColourRGB colour, RGB2Spec *rgb2spec)
{
    float rgb[3] = {static_cast<float>(colour.r), static_cast<float>(colour.g), static_cast<float>(colour.b)};
    rgb2spec_fetch(rgb2spec, rgb, coefficients_);
}

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

double Diffuse::albedo(const Ray &_ray, double lambda) const
{
    return rgb2spec_eval_precise(coefficients_, lambda);
};

double Diffuse::emitted(const Ray &_ray) const
{
    return 0.0;
};