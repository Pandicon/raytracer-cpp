#pragma once

#include <optional>

#include "Ray.hpp"
#include "Vec3.hpp"

struct ScatterRecord
{
    Ray scattered_ray;
    double albedo;
};

struct ComplexIOR
{
    /// @brief The wavelength is in micrometres, as it is at https://refractiveindex.info/
    double lambda;
    double n;
    double k;
};

class Metal
{
private:
    std::vector<ComplexIOR> complex_indices_of_refraction_;
    double roughness_;

    std::pair<double, double> find_index_of_refraction(double lambda) const;

public:
    Metal(std::vector<ComplexIOR> complex_indices_of_refraction, double roughness);

    std::optional<ScatterRecord> scatter(const Ray &ray, const Vec3 &hit_point, const Vec3 &normal, double n_previous, double lambda) const;

    double emitted(const Ray &_ray) const;

    // Values for specific metals come from https://refractiveindex.info/

    // Assumes lambda(r) = 656nm, lambda(g) = 532nm, lambda(b) = 450nm
    static Metal copper(double roughness);

    // Assumes lambda(r) = 656nm, lambda(g) = 532nm, lambda(b) = 450nm
    static Metal silver(double roughness);

    // Assumes lambda(r) = 656nm, lambda(g) = 532nm, lambda(b) = 450nm
    static Metal gold(double roughness);
};