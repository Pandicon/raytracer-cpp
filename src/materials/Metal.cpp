#include "materials/Metal.hpp"

namespace
{
#include "MetalDefaults.inc"
}

Metal::Metal(std::vector<ComplexIOR> complex_indices_of_refraction, double roughness) : complex_indices_of_refraction_(std::move(complex_indices_of_refraction)), roughness_(roughness)
{
    std::sort(complex_indices_of_refraction_.begin(), complex_indices_of_refraction_.end(),
              [](const ComplexIOR &a, const ComplexIOR &b)
              {
                  return a.lambda < b.lambda;
              });
}

std::pair<double, double> Metal::find_index_of_refraction(double lambda) const
{
    if (complex_indices_of_refraction_.empty())
    {
        return {1.0, 1.0};
    }
    const double lambda_micro_m = lambda / 1000.0;
    const auto lower_bound_iterator = std::lower_bound(complex_indices_of_refraction_.begin(), complex_indices_of_refraction_.end(), lambda_micro_m, [](const ComplexIOR &current_item, double desired_value)
                                                       { return current_item.lambda < desired_value; });

    if (lower_bound_iterator == complex_indices_of_refraction_.end())
    {
        return {complex_indices_of_refraction_.back().n, complex_indices_of_refraction_.back().k};
    }
    if (lower_bound_iterator == complex_indices_of_refraction_.begin())
    {
        return {lower_bound_iterator->n, lower_bound_iterator->k};
    }

    const auto previous_element = std::prev(lower_bound_iterator);

    const double t = (lambda_micro_m - previous_element->lambda) / (lower_bound_iterator->lambda - previous_element->lambda);

    const double n_int = previous_element->n + t * (lower_bound_iterator->n - previous_element->n);
    const double k_int = previous_element->k + t * (lower_bound_iterator->k - previous_element->k);

    return {n_int, k_int};
}

std::optional<ScatterRecord> Metal::scatter(const Ray &ray, const Vec3 &hit_point, const Vec3 &normal, double n_previous, double lambda) const
{
    const Vec3 reflected_direction = ray.direction.reflect(normal);
    const Vec3 new_direction = (reflected_direction + Vec3::random_unit_vector() * roughness_).normalise();
    if (new_direction.dot(normal) <= 0.0)
    {
        // TODO: This approximation is not really correct...
        return std::nullopt;
    }
    const Vec3 origin = hit_point;
    const Ray new_ray = Ray(origin, new_direction);

    const Vec3 effective_normal = (new_direction - ray.direction).normalise();

    const auto [n_val, k_val] = find_index_of_refraction(lambda);
    const double n_rel = n_val / n_previous;
    const double k_rel = k_val / n_previous;

    const double cos_theta_i = std::min(std::max(ray.direction.dot(effective_normal) * -1.0, 0.0), 1.0);
    const double cos_theta_i_2 = cos_theta_i * cos_theta_i;
    const double sin_theta_i_2 = std::max(0.0, 1.0 - cos_theta_i_2);
    const double sin_theta_i_4 = sin_theta_i_2 * sin_theta_i_2;

    const double n_2 = n_rel * n_rel;
    const double k_2 = k_rel * k_rel;

    const double n_2_minus_k_2_minus_s_theta_i_2 = n_2 - k_2 - sin_theta_i_2;

    const double a_2_plus_b_2 = std::sqrt(n_2_minus_k_2_minus_s_theta_i_2 * n_2_minus_k_2_minus_s_theta_i_2 + n_2 * k_2 * 4.0);

    const double a = std::sqrt((a_2_plus_b_2 + n_2_minus_k_2_minus_s_theta_i_2) * 0.5);

    const double r_s = (a_2_plus_b_2 - a * cos_theta_i * 2.0 + cos_theta_i_2) / (a_2_plus_b_2 + a * cos_theta_i * 2.0 + cos_theta_i_2);
    const double r_p = r_s * (a_2_plus_b_2 * cos_theta_i_2 - a * 2.0 * cos_theta_i * sin_theta_i_2 + sin_theta_i_4) / (a_2_plus_b_2 * cos_theta_i_2 + a * 2.0 * cos_theta_i * sin_theta_i_2 + sin_theta_i_4);

    const double f = (r_s + r_p) * 0.5;

    return ScatterRecord(new_ray, f);
}

double Metal::emitted(const Ray &_ray) const
{
    return 0.0;
};

Metal Metal::copper(double roughness)
{
    return Metal(COPPER_DATA, roughness);
}

Metal Metal::silver(double roughness)
{
    return Metal(SILVER_DATA, roughness);
}

Metal Metal::gold(double roughness)
{
    return Metal(GOLD_DATA, roughness);
}