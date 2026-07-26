#pragma once

#include "Interval.hpp"
#include "limits.hpp"
#include "Vec3.hpp"

struct Ray
{
    Vec3 origin;
    Vec3 direction;
    Interval valid_range;

    Ray(const Vec3 &origin_, const Vec3 &direction_) : origin(origin_), direction(direction_), valid_range(Interval(Limits::MINIMUM_T, Interval::universe_max())) {};

    Vec3 point_at(double t) const
    {
        return origin + direction * t;
    }
};