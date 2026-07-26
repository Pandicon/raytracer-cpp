#include "materials/BlackBody.hpp"

BlackBody::BlackBody(double temperature, double intensity) : temperature_(temperature), intensity_(intensity)
{
}

std::optional<Ray> BlackBody::scatter(const Ray &_ray, const Vec3 &_hit_point, const Vec3 &_normal) const
{
    // The ray hit a light source, it should not continue
    return std::nullopt;
}

double BlackBody::albedo(const Ray &_ray) const
{
    return 1.0;
};

double BlackBody::emitted(const Ray &_ray, double lambda) const
{
    double plack_radiation = 2 * hcc / (std::pow(lambda, 5) * (std::exp(hc_over_kb / (lambda * temperature_)) - 1));
    return plack_radiation * intensity_;
};