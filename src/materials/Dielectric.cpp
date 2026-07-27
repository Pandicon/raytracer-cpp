#include "materials/Dielectric.hpp"
#include "RandomUtils.hpp"

Dielectric::Dielectric(ColourRGB colour, const std::vector<double> &As, const std::vector<double> &Bs, RGB2Spec *rgb2spec) : num_terms_(static_cast<uint8_t>(std::min(As.size(), MAX_TERMS)))
{
    for (size_t i = 0; i < num_terms_; ++i)
    {
        As_[i] = As[i];
        Bs_[i] = Bs[i];
    }
    float rgb[3] = {static_cast<float>(colour.r), static_cast<float>(colour.g), static_cast<float>(colour.b)};
    rgb2spec_fetch(rgb2spec, rgb, coefficients_);
}

std::optional<Ray> Dielectric::scatter(const Ray &ray, const Vec3 &hit_point, const Vec3 &normal, double n_previous_over_n_next, bool &refracted) const
{
    Vec3 origin = hit_point;
    double minus_cos_theta = ray.direction.dot(normal);
    double cos_theta = -minus_cos_theta;
    double sin_theta_squared = std::fmax(0.0, 1.0 - minus_cos_theta * minus_cos_theta);
    double sin_theta_prime_squared = n_previous_over_n_next * n_previous_over_n_next * sin_theta_squared;
    if (sin_theta_prime_squared > 1.0)
    {
        // Total internal reflection
        refracted = false;
        Vec3 reflected_direction = ray.direction.reflect(normal);
        return Ray(origin, reflected_direction);
    }
    double cos_theta_prime = std::sqrt(std::fmax(0.0, 1.0 - sin_theta_prime_squared));
    double sqrt_r_s = (n_previous_over_n_next * cos_theta - cos_theta_prime) / (n_previous_over_n_next * cos_theta + cos_theta_prime);
    double sqrt_r_p = (n_previous_over_n_next * cos_theta_prime - cos_theta) / (n_previous_over_n_next * cos_theta_prime + cos_theta);
    double transmittance = 1 - 0.5 * (sqrt_r_p * sqrt_r_p + sqrt_r_s * sqrt_r_s);
    if (get_zero_to_one() > transmittance)
    {
        // Fresnel reflection
        refracted = false;
        Vec3 reflected_direction = ray.direction.reflect(normal);
        return Ray(origin, reflected_direction);
    }
    refracted = true;
    Vec3 d_perp = (ray.direction - normal * minus_cos_theta) * n_previous_over_n_next;
    Vec3 d_par = normal * (-cos_theta_prime);
    Vec3 refracted_direction = d_perp + d_par;
    return Ray(origin, refracted_direction);
}

double Dielectric::albedo(const Ray &_ray, double lambda) const
{
    return rgb2spec_eval_precise(coefficients_, lambda);
};

double Dielectric::emitted(const Ray &_ray) const
{
    return 0.0;
};