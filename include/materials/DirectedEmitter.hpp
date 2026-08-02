#pragma once

#include <optional>
#include <variant>

#include "BlackBody.hpp"
#include "Light.hpp"
#include "Vec3.hpp"

using Emitter = std::variant<BlackBody, Light>;

class DirectedEmitter
{
private:
    Emitter emitter_;
    double n_;
    double cos_theta_min_;
    double intensity_multiplier_;

public:
    DirectedEmitter(Emitter emitter, double n, double cos_theta_min);

    std::optional<Ray> scatter(const Ray &_ray, const Vec3 &_hit_point, const Vec3 &_normal) const;

    double albedo(const Ray &_ray) const;

    double emitted(const Ray &ray, const Vec3 &normal, double lambda) const;
};