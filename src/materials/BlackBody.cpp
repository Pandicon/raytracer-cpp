#include "materials/BlackBody.hpp"

BlackBody::BlackBody(double temperature, double intensity) : temperature_(temperature)
{
    constexpr double wiens_constant_nm = 2.89777e6;
    const double lambda_peak = wiens_constant_nm / temperature_;
    intensity_ = intensity / BlackBody::planck_radiation(lambda_peak, temperature);
}

std::optional<Ray> BlackBody::scatter(const Ray &_ray, const Vec3 &_hit_point, const Vec3 &_normal) const
{
    // The ray hit a light source, it should not continue
    return std::nullopt;
}

double BlackBody::albedo(const Ray &_ray) const
{
    return 1.0;
}

double BlackBody::emitted(const Ray &_ray, double lambda) const
{
    const double planck = BlackBody::planck_radiation(lambda, temperature_);
    return planck * intensity_;
}

double BlackBody::planck_radiation(double lambda, double temperature)
{
    return 2 * hcc / (std::pow(lambda, 5) * (std::exp(hc_over_kb / (lambda * temperature)) - 1));
}