#include "materials/Light.hpp"

Light::Light(ColourRGB colour, double intensity, RGB2Spec *rgb2spec) : intensity_(intensity)
{
    float rgb[3] = {static_cast<float>(colour.r), static_cast<float>(colour.g), static_cast<float>(colour.b)};
    rgb2spec_fetch(rgb2spec, rgb, coefficients_);
}

std::optional<Ray> Light::scatter(const Ray &_ray, const Vec3 &_hit_point, const Vec3 &_normal) const
{
    // The ray hit a light source, it should not continue
    return std::nullopt;
}

double Light::albedo(const Ray &_ray) const
{
    return 1.0;
};

double Light::emitted(const Ray &_ray, double lambda) const
{
    return rgb2spec_eval_precise(coefficients_, lambda) * intensity_;
};