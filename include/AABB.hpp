#pragma once

#include "Interval.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"

class AABB
{
private:
    Interval x_interval_;
    Interval y_interval_;
    Interval z_interval_;

public:
    AABB() = default;
    AABB(Interval x_interval, Interval y_interval, Interval z_interval);

    AABB extend_to_include(const Vec3 &position) const;

    AABB union_with(const AABB &other) const;

    bool intersects_ray(const Ray &ray) const;
};