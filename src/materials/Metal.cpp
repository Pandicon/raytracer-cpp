#include "materials/Metal.hpp"

Metal::Metal(Vec3 ns, Vec3 ks, double roughness) : ns_(ns), ks_(ks), roughness_(roughness) {}

std::optional<ScatterRecord> Metal::scatter(const Ray &ray, const Vec3 &hit_point, const Vec3 &normal, double n_previous) const
{
    const Vec3 reflected_direction = ray.direction.reflect(normal);
    const Vec3 new_direction = (reflected_direction + Vec3::random_unit_vector() * roughness_).normalise();
    if (new_direction.dot(normal) <= 0.0)
    {
        // TODO: This approximation is not really correct...
        return std::nullopt;
    }
    const Vec3 origin = hit_point;
    const Ray new_ray = Ray(origin, new_direction.normalise());

    const Vec3 effective_normal = (new_direction - ray.direction).normalise();

    const Vec3 ns_relative = ns_ / n_previous;
    const Vec3 ks_relative = ks_ / n_previous;

    const double cos_theta_i = std::min(std::max(ray.direction.dot(effective_normal) * -1.0, 0.0), 1.0);
    const double cos_theta_i_2 = cos_theta_i * cos_theta_i;
    const double sin_theta_i_2 = std::max(0.0, 1.0 - cos_theta_i_2);
    const double sin_theta_i_4 = sin_theta_i_2 * sin_theta_i_2;

    const Vec3 ns_2 = ns_relative * ns_relative;
    const Vec3 ks_2 = ks_relative * ks_relative;

    const Vec3 n_2_minus_k_2_minus_s_theta_i_2 = ns_2 - ks_2 - Vec3(sin_theta_i_2);

    const Vec3 a_2_plus_b_2 = (n_2_minus_k_2_minus_s_theta_i_2 * n_2_minus_k_2_minus_s_theta_i_2 + ns_2 * ks_2 * 4.0).sqrt();

    const Vec3 a = ((a_2_plus_b_2 + n_2_minus_k_2_minus_s_theta_i_2) * 0.5).sqrt();

    const Vec3 r_s = (a_2_plus_b_2 - a * cos_theta_i * 2.0 + Vec3(cos_theta_i_2)) / (a_2_plus_b_2 + a * cos_theta_i * 2.0 + Vec3(cos_theta_i_2));
    const Vec3 r_p = r_s * (a_2_plus_b_2 * cos_theta_i_2 - a * 2.0 * cos_theta_i * sin_theta_i_2 + sin_theta_i_4) / (a_2_plus_b_2 * cos_theta_i_2 + a * 2.0 * cos_theta_i * sin_theta_i_2 + sin_theta_i_4);

    const Vec3 f = (r_s + r_p) * 0.5;

    ColourRGB new_colour = ColourRGB(f.x, f.y, f.z);

    return ScatterRecord(new_ray, new_colour);
}

ColourRGB Metal::emitted(const Ray &_ray) const
{
    return ColourRGB::black();
};