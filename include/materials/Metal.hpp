#pragma once

#include <optional>

#include "Colour.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"

struct ScatterRecord
{
    Ray scattered_ray;
    ColourRGB colour_albedo;
};

class Metal
{
private:
    Vec3 ns_;
    Vec3 ks_;
    double roughness_;

public:
    Metal(Vec3 ns_, Vec3 ks_, double roughness);

    std::optional<ScatterRecord> scatter(const Ray &ray, const Vec3 &hit_point, const Vec3 &normal, double n_previous) const;

    ColourRGB emitted(const Ray &_ray) const;

    // Values for specific metals come from https://refractiveindex.info/

    // Assumes lambda(r) = 656nm, lambda(g) = 532nm, lambda(b) = 450nm
    static Metal copper(double roughness)
    {
        return Metal(Vec3(0.22656, 1.1159, 1.2404), Vec3(3.7026, 2.5956, 2.3929), roughness);
    }

    // Assumes lambda(r) = 656nm, lambda(g) = 532nm, lambda(b) = 450nm
    static Metal silver(double roughness)
    {
        return Metal(Vec3(0.050820, 0.054007, 0.040000), Vec3(4.4559, 3.4290, 2.6484), roughness);
    }

    // Assumes lambda(r) = 656nm, lambda(g) = 532nm, lambda(b) = 450nm
    static Metal gold(double roughness)
    {
        return Metal(Vec3(0.14574, 0.54386, 1.3831), Vec3(3.6622, 2.2309, 1.9155), roughness);
    }
};