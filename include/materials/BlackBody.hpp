#pragma once

#include <optional>

#include "Colour.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"

class BlackBody
{
private:
    static constexpr double hcc = 5.955214763e-17;
    static constexpr double hc_over_kb = 0.01438777354;

    double temperature_;
    double intensity_;

public:
    BlackBody(double temperature, double intensity);

    std::optional<Ray> scatter(const Ray &_ray, const Vec3 &_hit_point, const Vec3 &_normal) const;

    double albedo(const Ray &_ray) const;

    double emitted(const Ray &_ray, double lambda) const;
};