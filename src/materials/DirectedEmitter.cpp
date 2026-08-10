#include "materials/DirectedEmitter.hpp"

DirectedEmitter::DirectedEmitter(Emitter emitter, double n, double cos_theta_min) : emitter_(emitter), n_(n), cos_theta_min_(cos_theta_min)
{
    intensity_multiplier_ = (n + 2.0) / (2.0 * (1 - std::pow(cos_theta_min, n + 2.0)));
}

std::optional<Ray> DirectedEmitter::scatter(const Ray &_ray, const Vec3 &_hit_point, const Vec3 &_normal) const
{
    return std::visit([&](const auto &emitter)
                      { return emitter.scatter(_ray, _hit_point, _normal); }, emitter_);
}

double DirectedEmitter::albedo(const Ray &_ray) const
{
    return std::visit([&](const auto &emitter)
                      { return emitter.albedo(_ray); }, emitter_);
}

double DirectedEmitter::emitted(const Ray &ray, const Vec3 &normal, double lambda) const
{
    const double cos_theta = -ray.direction.dot(normal);
    if (cos_theta < cos_theta_min_)
    {
        return 0.0;
    }
    const double inner_emission = std::visit([&](const auto &emitter)
                                             { return emitter.emitted(ray, lambda); }, emitter_);
    const double scale = std::pow(cos_theta, n_) * intensity_multiplier_;
    return inner_emission * scale;
}