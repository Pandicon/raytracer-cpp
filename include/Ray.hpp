#pragma once
#include "Vec3.hpp"

struct Ray
{
    Vec3 origin;
    Vec3 direction;

    Ray(const Vec3 &origin_, const Vec3 &direction_) : origin(origin_), direction(direction_) {};

    Vec3 point_at(double t) const
    {
        return origin + direction * t;
    }
};