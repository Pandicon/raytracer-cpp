#pragma once

#include <functional>

#include "Vec3.hpp"

class Material;

struct HitRecord
{
    Vec3 point;
    Vec3 normal;
    // distance along the ray
    double t;
    // true if the ray hit the outside of the object, false if it hit the inside
    bool front_face;

    const Material *material;
};